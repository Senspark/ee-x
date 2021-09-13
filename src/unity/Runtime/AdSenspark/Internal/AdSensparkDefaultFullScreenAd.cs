using System;
using System.Threading.Tasks;
using UnityEngine;
using Jsonite;

using UnityEngine.Assertions;

using Object = UnityEngine.Object;

namespace EE.Internal {
    using Destroyer = Action;
    using ResultParser = Func<string, AdResult>;

    internal class AdSensparkDefaultFullScreenAd : ObserverManager<AdObserver>, IFullScreenAd {
        private const string kTag = nameof(AdSensparkDefaultFullScreenAd);
        private readonly string _prefix;
        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly IAsyncHelper<AdResult> _displayer;
        private readonly Destroyer _destroyer;
        private readonly ResultParser _resultParser;
        private readonly string _network;
        private readonly string _adId;
        private readonly MessageHelper _messageHelper;
        private readonly IAsyncHelper<bool> _loader;
        private readonly AdFormat _adFormat;
        private readonly AdSensparkResourceManager _adSensparkResourceManager;
        private int _adIndex;
        private AdSensparkCanvas _adCanvas;
        private AdSensparkResourcePack _adSensparkResourcePack;

        public AdSensparkDefaultFullScreenAd(
            string prefix,
            IMessageBridge bridge,
            ILogger logger,
            IAsyncHelper<AdResult> displayer,
            Destroyer destroyer,
            ResultParser resultParser,
            string network,
            string adId,
            AdFormat adFormat, 
            AdSensparkResourceManager adSensparkResourceManager, 
            JsonObject node) {
            _prefix = prefix;
            _bridge = bridge;
            _logger = logger;
            _displayer = displayer;
            _destroyer = destroyer;
            _resultParser = resultParser;
            _network = network;
            _adId = adId;
            _adFormat = adFormat;
            _messageHelper = new MessageHelper(_prefix, adId);
            _loader = new AsyncHelper<bool>();
            _adSensparkResourceManager = adSensparkResourceManager;
            
            _logger.Debug($"{kTag}: constructor: prefix = {_prefix} id = {_adId}");
            _bridge.RegisterHandler(_ => OnLoaded(), _messageHelper.OnLoaded);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnFailedToLoad((int) json["code"], (string) json["message"]);
            }, _messageHelper.OnFailedToLoad);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnFailedToShow((int) json["code"], (string) json["message"]);
            }, _messageHelper.OnFailedToShow);
            _bridge.RegisterHandler(_ => OnClicked(), _messageHelper.OnClicked);
            _bridge.RegisterHandler(message => {
                var result = _resultParser(message);
                OnClosed(result);
            }, _messageHelper.OnClosed);
            LoadDefaultResource(node);
            CreateAdSensparkPrefab(adFormat);
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
                case AdFormat.Interstitial:
                    _adCanvas.InitializeInterstitial(OnClicked, OnClosed, _adSensparkResourcePack.dataLocalPath);
                    break;

                case AdFormat.Rewarded:
                    _adCanvas.InitializeRewarded(OnClicked, OnClosed, _adSensparkResourcePack.dataLocalPath);
                    break;

                default:
                    Debug.LogWarning($"Ad senspark: chưa hỗ trợ {adFormat}.");
                    break;
            }
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: {nameof(Destroy)}: prefix = {_prefix} id = {_adId}");
            _bridge.DeregisterHandler(_messageHelper.OnLoaded);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
            _bridge.DeregisterHandler(_messageHelper.OnFailedToShow);
            _bridge.DeregisterHandler(_messageHelper.OnClicked);
            _bridge.DeregisterHandler(_messageHelper.OnClosed);
            _destroyer();
        }

        public bool IsLoaded {
            get => _adCanvas != null;
        }
        
        private void SetAdData() {
            _adCanvas.SetAdData(_adFormat, _adSensparkResourcePack);
        }
        
        public async Task<bool> Load() {
            _adSensparkResourcePack = await _adSensparkResourceManager.GetResource(_adFormat, _adIndex);

            _logger.Debug($"{kTag}: {nameof(Load)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            return true; // Vẫn báo true vì có default ad rồi.
        }

        public Task<AdResult> Show() {
            _adIndex++;
            _adCanvas.SetAdVisible(_adFormat, true);
            _logger.Debug(
                $"{kTag}: {nameof(Show)}: prefix = {_prefix} id = {_adId} displaying = {_displayer.IsProcessing}");
            return _displayer.Process(
                () => _bridge.Call(_messageHelper.Show),
                result => {
                    // OK.
                });
            // if(IsLoaded)
            //     return Task.FromResult(AdResult.Completed);
            // return Task.FromResult(AdResult.NotLoaded);
        }

        private void OnLoaded() {
            _logger.Debug(
                $"{kTag}: {nameof(OnLoaded)}: prefix = {_prefix} id = {_adId}.");
            if (_loader.IsProcessing) {
                _loader.Resolve(true);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = true
                }));
            } else {
                Assert.IsTrue(false);
            }
            SetAdData();
            DispatchEvent(observer => observer.OnLoaded?.Invoke());
        }

        private void OnFailedToLoad(int code, string message) {
            _logger.Debug(
                $"{kTag}: {nameof(OnFailedToLoad)}: prefix = {_prefix} id = {_adId}.");
            if (_loader.IsProcessing) {
                _loader.Resolve(false);
                DispatchEvent(observer => observer.OnLoadResult?.Invoke(new AdLoadResult {
                    Network = _network,
                    Result = false,
                    ErrorCode = code,
                    ErrorMessage = message
                }));
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnFailedToShow(int code, string message) {
            _logger.Debug(
                $"{kTag}: {nameof(OnFailedToLoad)}: prefix = {_prefix} id = {_adId}");
            if (_displayer.IsProcessing) {
                _displayer.Resolve(AdResult.Failed);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnClicked() {
            _logger.Debug($"{kTag}: {nameof(OnClicked)}: prefix = {_prefix} id = {_adId}");
            Application.OpenURL(_adSensparkResourcePack.promotionUrl);
            DispatchEvent(observer => observer.OnClicked?.Invoke());
        }

        private void OnClosed(AdResult result) {
            _logger.Debug(
                $"{kTag}: {nameof(OnClosed)}: prefix = {_prefix} id = {_adId}");
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
            } else {
                Assert.IsTrue(false);
            }
        }
    }
}