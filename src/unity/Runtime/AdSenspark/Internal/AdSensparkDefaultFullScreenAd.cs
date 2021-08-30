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
        private AdSensparkCanvas _adCanvas;

        public AdSensparkDefaultFullScreenAd(
            string prefix,
            IMessageBridge bridge,
            ILogger logger,
            IAsyncHelper<AdResult> displayer,
            Destroyer destroyer,
            ResultParser resultParser,
            string network,
            string adId,
            AdFormat adFormat) {
            _prefix = prefix;
            _bridge = bridge;
            _logger = logger;
            _displayer = displayer;
            _destroyer = destroyer;
            _resultParser = resultParser;
            _network = network;
            _adId = adId;
            _messageHelper = new MessageHelper(_prefix, adId);
            _loader = new AsyncHelper<bool>();

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
            LoadPrefab(adFormat);
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
                case AdFormat.AppOpen:
                    break;

                case AdFormat.Interstitial:
                    _adCanvas.InitializeInterstitial(OnClicked, OnClosed);
                    break;

                case AdFormat.RewardedInterstitial:
                    break;

                case AdFormat.Rewarded:
                    break;

                case AdFormat.Null:
                    break;

                default:
                    throw new ArgumentOutOfRangeException(nameof(adFormat), adFormat, null);
            }
            _logger.Debug($"{kTag}: load complete: prefix = {_prefix} id = {_adId}");
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

        public Task<bool> Load() {
            _logger.Debug($"{kTag}: {nameof(Load)}: prefix = {_prefix} id = {_adId} loading = {_loader.IsProcessing}");
            return Task.FromResult(true);
        }

        public Task<AdResult> Show() {
            _adCanvas.gameObject.SetActive(true);
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