using System;
using System.Threading.Tasks;
using UnityEngine;

using Jsonite;

using Object = UnityEngine.Object;

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
        private int _adIndex;
        private AdFormat _adFormat;
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
            AdSensparkResourceManager adSensparkResourceManager) {
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
            _adSensparkResourcePack = new AdSensparkResourcePack();
            LoadPrefab(adFormat);
            Utils.NoAwait(RefreshAd);
        }

        private void LoadPrefab(AdFormat adFormat) {
            var hadCanvas = ServiceLocatorSimple.ServiceAvailable<AdSensparkCanvas>();
            if (hadCanvas) {
                _adCanvas = ServiceLocatorSimple.GetService<AdSensparkCanvas>();
            } else {
                _logger.Debug($"{kTag}: loading prefab: prefix = {_prefix} id = {_adId}");
                var obj = Resources.Load<AdSensparkCanvas>("AdSenspark/AdSensparkCanvas");
                if (obj == null) {
                    _logger.Debug($"{kTag}: fail to load prefab: prefix = {_prefix} id = {_adId}");
                    return;
                }
                _adCanvas = Object.Instantiate(obj);
            }
            switch (adFormat) {
                case AdFormat.Banner:
                    _adCanvas.InitializeBanner(OnClicked, IsVisible);
                    break;

                case AdFormat.Rectangle:
                    break;
                
                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
            _logger.Debug($"{kTag}: load complete: prefix = {_prefix} id = {_adId}");
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
                await Task.Delay(60000);
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
            string url = "https://senspark.com/";
            if (_adSensparkResourcePack.IsNull()) {
                url = Application.platform == RuntimePlatform.Android
                    ? "https://play.google.com/store/apps/dev?id=7830868662152106484"
                    : "https://apps.apple.com/vn/developer/senspark-co-ltd/id560842775";
            } else {
                url = _adSensparkResourcePack.promotionUrl;
            }
            Application.OpenURL(url);
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }
    }
}