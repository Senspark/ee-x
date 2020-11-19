using System.Threading.Tasks;

using UnityEngine.Assertions;

namespace EE.Internal {
    internal class IronSource : IIronSource {
        private const string kPrefix = "IronSourceBridge";
        private const string kInitialize = kPrefix + "Initialize";
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
        private IronSourceInterstitialAd _interstitialAd;
        private IInterstitialAd _sharedInterstitialAd;
        private IronSourceRewardedAd _rewardedAd;
        private IRewardedAd _sharedRewardedAd;
        private readonly IAsyncHelper<bool> _interstitialAdDisplayer;
        private readonly IAsyncHelper<IRewardedAdResult> _rewardedAdDisplayer;

        public IronSource(IMessageBridge bridge) {
            _bridge = bridge;
            _interstitialAdDisplayer = MediationManager.Instance.InterstitialAdDisplayer;
            _rewardedAdDisplayer = MediationManager.Instance.RewardedAdDisplayer;

            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnInterstitialAdLoaded);
                return "";
            }, kOnInterstitialAdLoaded);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => OnInterstitialAdFailedToLoad(message));
                return "";
            }, kOnInterstitialAdFailedToLoad);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => OnInterstitialAdFailedToShow(message));
                return "";
            }, kOnInterstitialAdFailedToShow);
            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnInterstitialAdClicked);
                return "";
            }, kOnInterstitialAdClicked);
            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnInterstitialAdClosed);
                return "";
            }, kOnInterstitialAdClosed);
            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnRewardedAdLoaded);
                return "";
            }, kOnRewardedAdLoaded);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => OnRewardedAdFailedToShow(message));
                return "";
            }, kOnRewardedAdFailedToShow);
            _bridge.RegisterHandler(_ => {
                Thread.RunOnLibraryThread(OnRewardedAdClicked);
                return "";
            }, kOnRewardedAdClicked);
            _bridge.RegisterHandler(message => {
                Thread.RunOnLibraryThread(() => OnRewardedAdClosed(Utils.ToBool(message)));
                return "";
            }, kOnRewardedAdClosed);
        }

        public void Destroy() {
            _bridge.DeregisterHandler(kOnInterstitialAdLoaded);
            _bridge.DeregisterHandler(kOnInterstitialAdFailedToLoad);
            _bridge.DeregisterHandler(kOnInterstitialAdFailedToShow);
            _bridge.DeregisterHandler(kOnInterstitialAdClicked);
            _bridge.DeregisterHandler(kOnInterstitialAdClosed);
            _bridge.DeregisterHandler(kOnRewardedAdLoaded);
            _bridge.DeregisterHandler(kOnRewardedAdFailedToShow);
            _bridge.DeregisterHandler(kOnRewardedAdClicked);
            _bridge.DeregisterHandler(kOnRewardedAdClosed);
        }

        public async Task<bool> Initialize(string appKey) {
            var response = await _bridge.CallAsync(kInitialize, appKey);
            return Utils.ToBool(response);
        }

        public IInterstitialAd CreateInterstitialAd(string adId) {
            if (_sharedInterstitialAd != null) {
                return _sharedInterstitialAd;
            }
            _interstitialAd = new IronSourceInterstitialAd(_interstitialAdDisplayer, this, adId);
            _sharedInterstitialAd = new GuardedInterstitialAd(_interstitialAd);
            return _sharedInterstitialAd;
        }

        internal bool DestroyInterstitialAd(string adId) {
            if (_sharedInterstitialAd == null) {
                return false;
            }
            _interstitialAd = null;
            _sharedInterstitialAd = null;
            return true;
        }

        public IRewardedAd CreateRewardedAd(string adId) {
            if (_sharedRewardedAd != null) {
                return _sharedRewardedAd;
            }
            _rewardedAd = new IronSourceRewardedAd(_rewardedAdDisplayer, this, adId);
            _sharedRewardedAd = new GuardedRewardedAd(_rewardedAd);
            return _sharedRewardedAd;
        }

        internal bool DestroyRewardedAd(string adId) {
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
            _bridge.Call(kShowRewardedAd);
        }

        private void OnInterstitialAdLoaded() {
            if (_interstitialAd != null) {
                _interstitialAd.OnLoaded();
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnInterstitialAdFailedToLoad(string message) {
            if (_interstitialAd != null) {
                _interstitialAd.OnFailedToLoad(message);
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnInterstitialAdFailedToShow(string message) {
            if (_interstitialAd != null) {
                _interstitialAd.OnFailedToShow(message);
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
                OnMediationAdClosed(false);
            }
        }

        private void OnRewardedAdLoaded() {
            if (_rewardedAd != null) {
                _rewardedAd.OnLoaded();
            } else {
                Assert.IsTrue(false);
            }
        }

        private void OnRewardedAdFailedToShow(string message) {
            if (_rewardedAd != null) {
                _rewardedAd.OnFailedToShow(message);
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
                OnMediationAdClosed(rewarded);
            }
        }

        private void OnMediationAdClosed(bool rewarded) {
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