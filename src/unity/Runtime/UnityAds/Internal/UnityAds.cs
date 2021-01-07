using System;
using System.Collections.Generic;
using System.Threading.Tasks;

using UnityEngine;
using UnityEngine.Assertions;

namespace EE.Internal {
    public class UnityAds : IUnityAds {
        private const string kPrefix = "UnityAdsBridge";
        private const string kInitialize = kPrefix + "Initialize";
        private const string kSetDebugModeEnabled = kPrefix + "SetDebugModeEnabled";
        private const string kHasRewardedAd = kPrefix + "HasRewardedAd";
        private const string kLoadRewardedAd = kPrefix + "LoadRewardedAd";
        private const string kShowRewardedAd = kPrefix + "ShowRewardedAd";
        private const string kOnLoaded = kPrefix + "OnLoaded";
        private const string kOnFailedToShow = kPrefix + "OnFailedToShow";
        private const string kOnClosed = kPrefix + "OnClosed";

        private readonly IMessageBridge _bridge;
        private bool _displaying;
        private string _adId;
        private readonly Dictionary<string, (IAd, IAd)> _ads;
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;

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

        public UnityAds(IMessageBridge bridge) {
            _bridge = bridge;
            _displaying = false;
            _ads = new Dictionary<string, (IAd, IAd)>();
            _displayer = MediationManager.Instance.AdDisplayer;

            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => OnLoaded(message));
                return "";
            }, kOnLoaded);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => {
                    var response = JsonUtility.FromJson<OnFailedToShowResponse>(message);
                    OnFailedToShow(response.ad_id, response.message);
                });
                return "";
            }, kOnFailedToShow);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => {
                    var response = JsonUtility.FromJson<OnClosedResponse>(message);
                    OnClosed(response.ad_id, response.rewarded);
                });
                return "";
            }, kOnClosed);
        }

        public void Destroy() {
            _bridge.DeregisterHandler(kOnLoaded);
            _bridge.DeregisterHandler(kOnFailedToShow);
            _bridge.DeregisterHandler(kOnClosed);
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
            await Thread.SwitchToLibraryThread();
            return Utils.ToBool(response);
        }

        public void SetDebugModeEnabled(bool enabled) {
            _bridge.Call(kSetDebugModeEnabled, Utils.ToString(enabled));
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return CreateFullScreenAd(adId, () => new UnityInterstitialAd(_displayer, this, adId));
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return CreateFullScreenAd(adId, () => new UnityRewardedAd(_displayer, this, adId));
        }

        private IFullScreenAd CreateFullScreenAd(string adId, Func<IFullScreenAd> creator) {
            if (_ads.TryGetValue(adId, out var result)) {
                return result.Item1 as IFullScreenAd;
            }
            var raw = creator();
            var ad = new GuardedFullScreenAd(raw);
            _ads.Add(adId, (ad, raw));
            return ad;
        }

        internal bool DestroyAd(string adId) {
            if (!_ads.ContainsKey(adId)) {
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
            await Thread.SwitchToLibraryThread();
            return Utils.ToBool(response);
        }

        internal void ShowRewardedAd(string adId) {
            _adId = adId;
            _displaying = true;
            _bridge.Call(kShowRewardedAd, adId);
        }

        private void OnLoaded(string adId) {
            if (_ads.TryGetValue(adId, out var iter)) {
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
                if (_ads.TryGetValue(adId, out var iter)) {
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
                if (_ads.TryGetValue(adId, out var iter)) {
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
                    ? FullScreenAdResult.Completed
                    : FullScreenAdResult.Canceled);
            }
        }

        private void OnMediationAdFailedToShow(string adId, string message) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(FullScreenAdResult.Failed);
                return;
            }
            Assert.IsTrue(false);
        }

        private void OnMediationAdClosed(string adId, FullScreenAdResult result) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
                return;
            }
            Assert.IsTrue(false);
        }
    }
}