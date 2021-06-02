using System;
using System.Threading.Tasks;

using Jsonite;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;

    internal class IronSource : IIronSource {
        private const string kTag = nameof(IronSource);
        private const string kPrefix = "IronSourceBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kGetBannerAdSize = kPrefix + "GetBannerAdSize";
        private const string kCreateBannerAd = kPrefix + "CreateBannerAd";
        private const string kDestroyAd = kPrefix + "DestroyAd";
        private const string kHasInterstitialAd = kPrefix + "HasInterstitialAd";
        private const string kLoadInterstitialAd = kPrefix + "LoadInterstitialAd";
        private const string kShowInterstitialAd = kPrefix + "ShowInterstitialAd";
        private const string kHasRewardedAd = kPrefix + "HasRewardedAd";
        private const string kShowRewardedAd = kPrefix + "ShowRewardedAd";
        private const string kOnInterstitialAdLoaded = kPrefix + "OnInterstitialAdLoaded";
        private const string kOnInterstitialAdFailedToLoad = kPrefix + "OnInterstitialAdFailedToLoad";
        private const string kOnInterstitialAdFailedToShow = kPrefix + "OnInterstitialAdFailedToShow";
        private const string kOnInterstitialAdClicked = kPrefix + "OnInterstitialAdClicked";
        private const string kOnInterstitialAdClosed = kPrefix + "OnInterstitialAdClosed";
        private const string kOnRewardedAdLoaded = kPrefix + "OnRewardedAdLoaded";
        private const string kOnRewardedAdFailedToShow = kPrefix + "OnRewardedAdFailedToShow";
        private const string kOnRewardedAdClicked = kPrefix + "OnRewardedAdClicked";
        private const string kOnRewardedAdClosed = kPrefix + "OnRewardedAdClosed";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;
        private readonly string _network;
        private IBannerAd _bannerAd;
        private IronSourceInterstitialAd _interstitialAd;
        private IFullScreenAd _sharedInterstitialAd;
        private IronSourceRewardedAd _rewardedAd;
        private IFullScreenAd _sharedRewardedAd;
        private readonly IAsyncHelper<AdResult> _displayer;

        public IronSource(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
            _network = "iron_source";
            _displayer = MediationManager.Instance.AdDisplayer;

            _bridge.RegisterHandler(_ => OnInterstitialAdLoaded(), kOnInterstitialAdLoaded);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnInterstitialAdFailedToLoad((int) json["code"], (string) json["message"]);
            }, kOnInterstitialAdFailedToLoad);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnInterstitialAdFailedToShow((int) json["code"], (string) json["message"]);
            }, kOnInterstitialAdFailedToShow);
            _bridge.RegisterHandler(_ => OnInterstitialAdClicked(), kOnInterstitialAdClicked);
            _bridge.RegisterHandler(_ => OnInterstitialAdClosed(), kOnInterstitialAdClosed);
            _bridge.RegisterHandler(_ => OnRewardedAdLoaded(), kOnRewardedAdLoaded);
            _bridge.RegisterHandler(message => {
                var json = (JsonObject) Json.Deserialize(message);
                OnRewardedAdFailedToShow((int) json["code"], (string) json["message"]);
            }, kOnRewardedAdFailedToShow);
            _bridge.RegisterHandler(_ => OnRewardedAdClicked(), kOnRewardedAdClicked);
            _bridge.RegisterHandler(message => OnRewardedAdClosed(Utils.ToBool(message)), kOnRewardedAdClosed);
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            _bridge.DeregisterHandler(kOnInterstitialAdLoaded);
            _bridge.DeregisterHandler(kOnInterstitialAdFailedToLoad);
            _bridge.DeregisterHandler(kOnInterstitialAdFailedToShow);
            _bridge.DeregisterHandler(kOnInterstitialAdClicked);
            _bridge.DeregisterHandler(kOnInterstitialAdClosed);
            _bridge.DeregisterHandler(kOnRewardedAdLoaded);
            _bridge.DeregisterHandler(kOnRewardedAdFailedToShow);
            _bridge.DeregisterHandler(kOnRewardedAdClicked);
            _bridge.DeregisterHandler(kOnRewardedAdClosed);
            _destroyer();
        }

        public async Task<bool> Initialize(string appKey) {
            var response = await _bridge.CallAsync(kInitialize, appKey);
            return Utils.ToBool(response);
        }

        [Serializable]
        private struct GetBannerAdSizeResponse {
            public int width;
            public int height;
        }

        private (int, int) GetBannerAdSize(IronSourceBannerAdSize adSize) {
            var response = _bridge.Call(kGetBannerAdSize, ((int) adSize).ToString());
            var json = JsonUtility.FromJson<GetBannerAdSizeResponse>(response);
            return (json.width, json.height);
        }

        [Serializable]
        private struct CreateBannerAdRequest {
            public string adId;
            public int adSize;
        }

        public IBannerAd CreateBannerAd(string adId, IronSourceBannerAdSize adSize) {
            _logger.Debug($"{kTag}: {nameof(CreateBannerAd)}: id = {adId} size = {adSize}");
            if (_bannerAd != null) {
                return _bannerAd;
            }
            var request = new CreateBannerAdRequest {
                adId = adId,
                adSize = (int) adSize
            };
            var response = _bridge.Call(kCreateBannerAd, JsonUtility.ToJson(request));
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return null;
            }
            var size = GetBannerAdSize(adSize);
            _bannerAd = new GuardedBannerAd(new DefaultBannerAd("IronSourceBannerAd", _bridge, _logger,
                    () => DestroyBannerAd(adId), _network, adId, size),
                new Capper(10f), new Retrier(3f, 3f, 30f));
            return _bannerAd;
        }

        private bool DestroyBannerAd(string adId) {
            _logger.Debug($"{kTag}: {nameof(DestroyBannerAd)}: id = {adId}");
            if (_bannerAd == null) {
                return false;
            }
            var response = _bridge.Call(kDestroyAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _bannerAd = null;
            return true;
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            _logger.Debug($"{kTag}: {nameof(CreateInterstitialAd)}: id = {adId}");
            if (_sharedInterstitialAd != null) {
                return _sharedInterstitialAd;
            }
            _interstitialAd = new IronSourceInterstitialAd(_displayer, this, _network, adId);
            _sharedInterstitialAd = new GuardedFullScreenAd(_interstitialAd, new Capper(10f), new Retrier(3f, 3f, 30f));
            return _sharedInterstitialAd;
        }

        internal bool DestroyInterstitialAd(string adId) {
            _logger.Debug($"{kTag}: {nameof(DestroyInterstitialAd)}: id = {adId}");
            if (_sharedInterstitialAd == null) {
                return false;
            }
            _interstitialAd = null;
            _sharedInterstitialAd = null;
            return true;
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            if (_sharedRewardedAd != null) {
                return _sharedRewardedAd;
            }
            _rewardedAd = new IronSourceRewardedAd(_displayer, this, _network, adId);
            _sharedRewardedAd = new GuardedFullScreenAd(_rewardedAd, new Capper(10f), new Retrier(3f, 3f, 30f));
            return _sharedRewardedAd;
        }

        internal bool DestroyRewardedAd(string adId) {
            _logger.Debug($"{kTag}: {nameof(DestroyRewardedAd)}: id = {adId}");
            if (_sharedRewardedAd == null) {
                return false;
            }
            _rewardedAd = null;
            _sharedRewardedAd = null;
            return true;
        }

        internal bool HasInterstitialAd() {
            var response = _bridge.Call(kHasInterstitialAd);
            return Utils.ToBool(response);
        }

        internal void LoadInterstitialAd() {
            _bridge.Call(kLoadInterstitialAd);
        }

        internal void ShowInterstitialAd(string adId) {
            _bridge.Call(kShowInterstitialAd, adId);
        }

        internal bool HasRewardedAd() {
            var response = _bridge.Call(kHasRewardedAd);
            return Utils.ToBool(response);
        }

        internal void ShowRewardedAd(string adId) {
            _bridge.Call(kShowRewardedAd, adId);
        }

        private void OnInterstitialAdLoaded() {
            if (_interstitialAd != null) {
                _interstitialAd.OnLoaded();
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnInterstitialAdFailedToLoad(int code, string message) {
            if (_interstitialAd != null) {
                _interstitialAd.OnFailedToLoad(code, message);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnInterstitialAdFailedToShow(int code, string message) {
            if (_interstitialAd != null) {
                _interstitialAd.OnFailedToShow(code, message);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnInterstitialAdClicked() {
            if (_interstitialAd != null) {
                _interstitialAd.OnClicked();
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnInterstitialAdClosed() {
            if (_interstitialAd != null) {
                _interstitialAd.OnClosed();
            } else {
                OnMediationAdClosed(AdResult.Completed);
            }
        }

        private void OnRewardedAdLoaded() {
            if (_rewardedAd != null) {
                _rewardedAd.OnLoaded();
            } else {
                // Automatically reloaded by SDK.
            }
        }

        private void OnRewardedAdFailedToShow(int code, string message) {
            if (_rewardedAd != null) {
                _rewardedAd.OnFailedToShow(code, message);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnRewardedAdClicked() {
            if (_rewardedAd != null) {
                _rewardedAd.OnClicked();
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnRewardedAdClosed(bool rewarded) {
            if (_rewardedAd != null) {
                _rewardedAd.OnClosed(rewarded);
            } else {
                OnMediationAdClosed(rewarded
                    ? AdResult.Completed
                    : AdResult.Canceled);
            }
        }

        private void OnMediationAdClosed(AdResult result) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
                return;
            }
            Assert.IsTrue(false);
        }
    }
}