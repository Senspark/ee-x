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
        private const string kShowRewardedAd = kPrefix + "ShowRewardedAd";
        private const string kOnLoaded = kPrefix + "OnLoaded";
        private const string kOnFailedToShow = kPrefix + "OnFailedToShow";
        private const string kOnClosed = kPrefix + "OnClosed";

        private readonly IMessageBridge _bridge;
        private bool _displaying;
        private string _adId;
        private readonly Dictionary<string, (IInterstitialAd, UnityInterstitialAd)> _interstitialAds;
        private readonly Dictionary<string, (IRewardedAd, UnityRewardedAd)> _rewardedAds;
        private readonly IAsyncHelper<bool> _interstitialAdDisplayer;
        private readonly IAsyncHelper<IRewardedAdResult> _rewardedAdDisplayer;

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
            _interstitialAds = new Dictionary<string, (IInterstitialAd, UnityInterstitialAd)>();
            _rewardedAds = new Dictionary<string, (IRewardedAd, UnityRewardedAd)>();
            _interstitialAdDisplayer = MediationManager.Instance.InterstitialAdDisplayer;
            _rewardedAdDisplayer = MediationManager.Instance.RewardedAdDisplayer;

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

        public IInterstitialAd CreateInterstitialAd(string adId) {
            if (_interstitialAds.TryGetValue(adId, out var result)) {
                return result.Item1;
            }
            var weak = new UnityInterstitialAd(_interstitialAdDisplayer, this, adId);
            var strong = new GuardedInterstitialAd(weak);
            _interstitialAds.Add(adId, (strong, weak));
            return strong;
        }

        internal bool DestroyInterstitialAd(string adId) {
            if (!_interstitialAds.ContainsKey(adId)) {
                return false;
            }
            _interstitialAds.Remove(adId);
            return true;
        }

        public IRewardedAd CreateRewardedAd(string adId) {
            if (_rewardedAds.TryGetValue(adId, out var result)) {
                return result.Item1;
            }
            var weak = new UnityRewardedAd(_rewardedAdDisplayer, this, adId);
            var strong = new GuardedRewardedAd(weak);
            _rewardedAds.Add(adId, (strong, weak));
            return strong;
        }

        internal bool DestroyRewardedAd(string adId) {
            if (!_rewardedAds.ContainsKey(adId)) {
                return false;
            }
            _rewardedAds.Remove(adId);
            return true;
        }

        internal bool HasRewardedAd(string adId) {
            var response = _bridge.Call(kHasRewardedAd, adId);
            return Utils.ToBool(response);
        }

        internal void ShowRewardedAd(string adId) {
            _adId = adId;
            _displaying = true;
            _bridge.Call(kShowRewardedAd, adId);
        }

        private void OnLoaded(string adId) {
            if (_interstitialAds.TryGetValue(adId, out var interstitialAd)) {
                interstitialAd.Item2.OnLoaded();
                return;
            }
            if (_rewardedAds.TryGetValue(adId, out var rewardedAd)) {
                rewardedAd.Item2.OnLoaded();
                return;
            }
            // Mediation.
        }

        private void OnFailedToShow(string adId, string message) {
            if (_displaying) {
                Assert.IsTrue(adId == _adId);
                if (_interstitialAds.TryGetValue(adId, out var interstitialAd)) {
                    interstitialAd.Item2.OnFailedToShow(message);
                }
                if (_rewardedAds.TryGetValue(adId, out var rewardedAd)) {
                    rewardedAd.Item2.OnFailedToShow(message);
                }
                _displaying = false;
            } else {
                OnMediationAdFailedToShow(adId, message);
            }
        }

        private void OnClosed(string adId, bool rewarded) {
            if (_displaying) {
                Assert.IsTrue(adId == _adId);
                if (_interstitialAds.TryGetValue(adId, out var interstitialAd)) {
                    interstitialAd.Item2.OnClosed();
                }
                if (_rewardedAds.TryGetValue(adId, out var rewardedAd)) {
                    rewardedAd.Item2.OnClosed(rewarded);
                }
            } else {
                OnMediationAdClosed(adId, rewarded);
            }
        }

        private void OnMediationAdFailedToShow(string adId, string message) {
            if (_interstitialAdDisplayer.IsProcessing) {
                _interstitialAdDisplayer.Resolve(false);
                return;
            }
            if (_rewardedAdDisplayer.IsProcessing) {
                _rewardedAdDisplayer.Resolve(IRewardedAdResult.Failed);
                return;
            }
            Assert.IsTrue(false);
        }

        private void OnMediationAdClosed(string adId, bool rewarded) {
            if (_interstitialAdDisplayer.IsProcessing) {
                _interstitialAdDisplayer.Resolve(true);
                return;
            }
            if (_rewardedAdDisplayer.IsProcessing) {
                _rewardedAdDisplayer.Resolve(rewarded
                    ? IRewardedAdResult.Completed
                    : IRewardedAdResult.Canceled);
                return;
            }
            Assert.IsTrue(false);
        }
    }
}