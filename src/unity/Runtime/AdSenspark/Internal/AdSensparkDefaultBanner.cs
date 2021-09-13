using System;
using System.Threading.Tasks;
using UnityEngine;

using Jsonite;

namespace EE.Internal {
    using Destroyer = Action;

    internal class AdSensparkDefaultBanner : ObserverManager<AdObserver>, IBannerAd {
        private const string kTag = nameof(AdSensparkDefaultBanner);
        private readonly string _prefix;
        private readonly string _adId;
        private readonly string _network;
        private readonly Destroyer _destroyer;
        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly BannerAdHelper _helper;
        private readonly IAsyncHelper<bool> _loader;
        private readonly MessageHelper _messageHelper;
        private readonly AdSensparkResourceManager _adSensparkResourceManager;
        private readonly AdFormat _adFormat;
        private int _adIndex;
        private AdSensparkCanvas _adCanvas;
        private AdSensparkResourcePack _adSensparkResourcePack;

        public AdSensparkDefaultBanner(
            string prefix,
            IMessageBridge bridge,
            ILogger logger,
            Destroyer destroyer,
            string network,
            string adId,
            AdFormat adFormat,
            (int, int) size,
            AdSensparkResourceManager adSensparkResourceManager,
            JsonObject node) {
            _prefix = prefix;
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _network = network;
            _adId = adId;
            _adFormat = adFormat;
            _messageHelper = new MessageHelper(prefix, adId);
            _helper = new BannerAdHelper(_bridge, _messageHelper, size);
            _loader = new AsyncHelper<bool>();
            _logger.Debug($"{kTag}: constructor: prefix = {_prefix} id = {_adId}");
            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnFailedToLoad((int) json["code"], (string) json["message"]);
            }, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
            _adSensparkResourceManager = adSensparkResourceManager;
            LoadDefaultResource(node);
            CreateAdSensparkPrefab(adFormat);
            var t = RefreshAd();
        }

        private void LoadDefaultResource(JsonObject node) {
            var getPath = node.TryGetValue("image_local_path", out var path);
            var getUrl = node.TryGetValue(Application.platform == RuntimePlatform.Android ? 
                "promotion_url_android" : "promotion_url_ios", out var url);
            if (!getPath) {
                Debug.LogWarning($"Ad senspark: chưa cấu hình image_local_path trong file json cho {_adFormat}");
                return;
            }
            if (!getUrl) {
                Debug.LogWarning($"Ad senspark: chưa cấu hình promotion_url trong file json cho {_adFormat}");
                return;
            }
            _adSensparkResourcePack = new AdSensparkResourcePack {
                promotionUrl = (string) url,
                dataLocalPath = (string) path
            };
        }

        private void CreateAdSensparkPrefab(AdFormat adFormat) {
            _adCanvas = ServiceLocatorSimple.GetService<AdSensparkCanvas>();
            switch (adFormat) {
                case AdFormat.Banner:
                    _adCanvas.InitializeBanner(OnClicked, IsVisible, _adSensparkResourcePack.dataLocalPath);
                    break;

                default:
                    Debug.LogWarning($"Ad senspark: chưa hỗ trợ {adFormat}.");
                    break;
            }
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor: prefix = {_prefix} id = {_adId}");
            _bridge.DeregisterHandler(_messageHelper.OnLoaded);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
            _bridge.DeregisterHandler(_messageHelper.OnClicked);
            _destroyer();
        }

        private async Task RefreshAd() {
            while (true) {
                await Task.Delay(30000);
                if(!IsVisible)
                    continue;
                _adIndex++;
                await Load();
                SetAdData();
            }
        }

        public bool IsLoaded {
            get => _adCanvas != null;
        }

        public async Task<bool> Load() {
            _adSensparkResourcePack = await _adSensparkResourceManager.GetResource(_adFormat, _adIndex);
            OnLoaded(); // Không được điều hướng từ EE nên phải tự gọi.
            return true; // Vẫn báo true vì có default ad rồi.
        }

        public bool IsVisible {
            get => _helper.IsVisible;
            set {
                _helper.IsVisible = value;
                _adCanvas.SetAdVisible(_adFormat, value);
            }
        }

        public (float, float) Anchor {
            get => _helper.Anchor;
            set {
                _helper.Anchor = value;
                _adCanvas.SetAnchorBanner(value);
            }
        }

        public (float, float) Position {
            get => _helper.Position;
            set {
                _helper.Position = value;
                _adCanvas.SetPositionBanner(value);
            }
        }

        public (float, float) Size {
            get => _helper.Size;
            set {
                _helper.Size = value;
                _adCanvas.SetSizeBanner(value);
            }
        }

        private void SetAdData() {
            _adCanvas.SetAdData(_adFormat, _adSensparkResourcePack);
        }

        private void OnLoaded() {
            _logger.Debug(
                $"{kTag}: {nameof(OnLoaded)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = true
                }));
            } else {
                // Ignored.
            }
            SetAdData();
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        private void OnFailedToLoad(int code, string message) {
            _logger.Debug(
                $"{kTag}: {nameof(OnFailedToLoad)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing} message = {message}");
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = false,
                    ErrorCode = code,
                    ErrorMessage = message
                }));
            } else {
                // Ignored.
            }
        }

        private void OnClicked() {
            _logger.Debug($"{kTag}: {nameof(OnClicked)}: prefix = {_prefix} id = {_adId}");
            Application.OpenURL(_adSensparkResourcePack.promotionUrl);
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }
    }
}