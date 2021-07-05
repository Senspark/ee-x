using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    using Destroyer = Action;

    internal class UnityAds : IUnityAds {
        private const string kTag = nameof(UnityAds);
        private const string kPrefix = "UnityAdsBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kSetDebugModeEnabled = kPrefix + "SetDebugModeEnabled";
        private const string kGetBannerAdSize = kPrefix + "GetBannerAdSize";
        private const string kCreateBannerAd = kPrefix + "CreateBannerAd";
        private const string kDestroyAd = kPrefix + "DestroyAd";
        private const string kHasRewardedAd = kPrefix + "HasRewardedAd";
        private const string kLoadRewardedAd = kPrefix + "LoadRewardedAd";
        private const string kShowRewardedAd = kPrefix + "ShowRewardedAd";
        private const string kOnLoaded = kPrefix + "OnLoaded";
        private const string kOnFailedToShow = kPrefix + "OnFailedToShow";
        private const string kOnClosed = kPrefix + "OnClosed";

        private readonly IMessageBridge _bridge;
        private readonly ILogger _logger;
        private readonly Destroyer _destroyer;
        private readonly string _network;
        private bool _displaying;
        private string _adId;
        private readonly Dictionary<string, IAd> _ads;
        private readonly Dictionary<string, (IAd, IAd)> _fullScreenAds;
        private readonly IAsyncHelper<AdResult> _displayer;

        [Serializable]
        private struct OnFailedToShowResponse {
            public string ad_id;
            public string message;
        }

        [Serializable]
        private struct OnClosedResponse {
            public string ad_id;
            public bool rewarded;
        }

        public UnityAds(IMessageBridge bridge, ILogger logger, Destroyer destroyer) {
            _bridge = bridge;
            _logger = logger;
            _destroyer = destroyer;
            _logger.Debug($"{kTag}: constructor");
            _network = "unity_ads";
            _displaying = false;
            _ads = new Dictionary<string, IAd>();
            _fullScreenAds = new Dictionary<string, (IAd, IAd)>();
            _displayer = MediationManager.Instance.AdDisplayer;

            _bridge.RegisterHandler(OnLoaded, kOnLoaded);
            _bridge.RegisterHandler(message => {
                var response = JsonUtility.FromJson<OnFailedToShowResponse>(message);
                OnFailedToShow(response.ad_id, response.message);
            }, kOnFailedToShow);
            _bridge.RegisterHandler(message => {
                var response = JsonUtility.FromJson<OnClosedResponse>(message);
                OnClosed(response.ad_id, response.rewarded);
            }, kOnClosed);
        }

        public void Destroy() {
            _logger.Debug($"{kTag}: constructor");
            _bridge.DeregisterHandler(kOnLoaded);
            _bridge.DeregisterHandler(kOnFailedToShow);
            _bridge.DeregisterHandler(kOnClosed);
            foreach (var ad in _ads.Values) {
                ad.Destroy();
            }
            _ads.Clear();
            _destroyer();
        }

        [Serializable]
        private struct InitializeRequest {
            public string gameId;
            public bool testModeEnabled;
        }

        public async Task<bool> Initialize(string gameId, bool testModeEnabled) {
            var request = new InitializeRequest {
                gameId = gameId,
                testModeEnabled = testModeEnabled
            };
            var response = await _bridge.CallAsync(kInitialize, JsonUtility.ToJson(request));
            return Utils.ToBool(response);
        }

        public void SetDebugModeEnabled(bool enabled) {
            _bridge.Call(kSetDebugModeEnabled, Utils.ToString(enabled));
        }

        [Serializable]
        private struct GetBannerAdSizeResponse {
            public int width;
            public int height;
        }

        private (int, int) GetBannerAdSize(UnityBannerAdSize adSize) {
            var response = _bridge.Call(kGetBannerAdSize, ((int) adSize).ToString());
            var json = JsonUtility.FromJson<GetBannerAdSizeResponse>(response);
            return (json.width, json.height);
        }

        [Serializable]
        private struct CreateBannerAdRequest {
            public string adId;
            public int adSize;
        }

        public IBannerAd CreateBannerAd(string adId, UnityBannerAdSize adSize) {
            _logger.Debug($"{kTag}: {nameof(CreateBannerAd)}: id = {adId} size = {adSize}");
            if (_ads.TryGetValue(adId, out var result)) {
                return result as IBannerAd;
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
            var ad = new GuardedBannerAd(new DefaultBannerAd("UnityBannerAd", _bridge, _logger,
                () => DestroyAd(adId), _network, adId, size));
            _ads.Add(adId, ad);
            return ad;
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return CreateFullScreenAd(adId, () => new UnityInterstitialAd(_displayer, this, adId));
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return CreateFullScreenAd(adId, () => new UnityRewardedAd(_displayer, this, adId));
        }

        private IFullScreenAd CreateFullScreenAd(string adId, Func<IFullScreenAd> creator) {
            if (_fullScreenAds.TryGetValue(adId, out var result)) {
                return result.Item1 as IFullScreenAd;
            }
            var raw = creator();
            var ad = new GuardedFullScreenAd(raw);
            _fullScreenAds.Add(adId, (ad, raw));
            return ad;
        }

        internal bool DestroyFullScreenAd(string adId) {
            if (!_fullScreenAds.ContainsKey(adId)) {
                return false;
            }
            _fullScreenAds.Remove(adId);
            return true;
        }

        private bool DestroyAd(string adId) {
            _logger.Debug($"{kTag}: {nameof(DestroyAd)}: id = {adId}");
            if (!_ads.ContainsKey(adId)) {
                return false;
            }
            var response = _bridge.Call(kDestroyAd, adId);
            if (!Utils.ToBool(response)) {
                Assert.IsTrue(false);
                return false;
            }
            _ads.Remove(adId);
            return true;
        }

        internal bool HasRewardedAd(string adId) {
            var response = _bridge.Call(kHasRewardedAd, adId);
            return Utils.ToBool(response);
        }

        internal async Task<bool> LoadRewardedAd(string adId) {
            var response = await _bridge.CallAsync(kLoadRewardedAd, adId);
            return Utils.ToBool(response);
        }

        internal void ShowRewardedAd(string adId) {
            _adId = adId;
            _displaying = true;
            _bridge.Call(kShowRewardedAd, adId);
        }

        private void OnLoaded(string adId) {
            if (_fullScreenAds.TryGetValue(adId, out var iter)) {
                var (_, ad) = iter;
                switch (ad) {
                    case UnityInterstitialAd item:
                        item.OnLoaded();
                        return;
                    case UnityRewardedAd item:
                        item.OnLoaded();
                        return;
                }
            }
            // Mediation.
        }

        private void OnFailedToShow(string adId, string message) {
            if (_displaying) {
                Assert.IsTrue(adId == _adId);
                _displaying = false;
                if (_fullScreenAds.TryGetValue(adId, out var iter)) {
                    var (_, ad) = iter;
                    switch (ad) {
                        case UnityInterstitialAd item:
                            item.OnFailedToShow(message);
                            return;
                        case UnityRewardedAd item:
                            item.OnFailedToShow(message);
                            return;
                    }
                }
            } else {
                OnMediationAdFailedToShow(adId, message);
            }
        }

        private void OnClosed(string adId, bool rewarded) {
            if (_displaying) {
                Assert.IsTrue(adId == _adId);
                _displaying = false;
                if (_fullScreenAds.TryGetValue(adId, out var iter)) {
                    var (_, ad) = iter;
                    switch (ad) {
                        case UnityInterstitialAd item:
                            item.OnClosed();
                            return;
                        case UnityRewardedAd item:
                            item.OnClosed(rewarded);
                            return;
                    }
                }
            } else {
                OnMediationAdClosed(adId, rewarded
                    ? AdResult.Completed
                    : AdResult.Canceled);
            }
        }

        private void OnMediationAdFailedToShow(string adId, string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(AdResult.Failed);
                return;
            }
            Assert.IsTrue(false);
        }

        private void OnMediationAdClosed(string adId, AdResult result) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
                return;
            }
            Assert.IsTrue(false);
        }
    }
}