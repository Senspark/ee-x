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
        private IFullScreenAd _sharedInterstitialAd;
        private IronSourceRewardedAd _rewardedAd;
        private IFullScreenAd _sharedRewardedAd;
        private readonly IAsyncHelper<FullScreenAdResult> _displayer;

        public IronSource(IMessageBridge bridge) {
            _bridge = bridge;
            _displayer = MediationManager.Instance.AdDisplayer;

            _bridge.RegisterHandler(_ => OnInterstitialAdLoaded(), kOnInterstitialAdLoaded);
            _bridge.RegisterHandler(OnInterstitialAdFailedToLoad, kOnInterstitialAdFailedToLoad);
            _bridge.RegisterHandler(OnInterstitialAdFailedToShow, kOnInterstitialAdFailedToShow);
            _bridge.RegisterHandler(_ => OnInterstitialAdClicked(), kOnInterstitialAdClicked);
            _bridge.RegisterHandler(_ => OnInterstitialAdClosed(), kOnInterstitialAdClosed);
            _bridge.RegisterHandler(_ => OnRewardedAdLoaded(), kOnRewardedAdLoaded);
            _bridge.RegisterHandler(OnRewardedAdFailedToShow, kOnRewardedAdFailedToShow);
            _bridge.RegisterHandler(_ => OnRewardedAdClicked(), kOnRewardedAdClicked);
            _bridge.RegisterHandler(message => OnRewardedAdClosed(Utils.ToBool(message)), kOnRewardedAdClosed);
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

        public IFullScreenAd CreateInterstitialAd(string adId) {
            if (_sharedInterstitialAd != null) {
                return _sharedInterstitialAd;
            }
            _interstitialAd = new IronSourceInterstitialAd(_displayer, this, adId);
            _sharedInterstitialAd = new GuardedFullScreenAd(_interstitialAd);
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

        public IFullScreenAd CreateRewardedAd(string adId) {
            if (_sharedRewardedAd != null) {
                return _sharedRewardedAd;
            }
            _rewardedAd = new IronSourceRewardedAd(_displayer, this, adId);
            _sharedRewardedAd = new GuardedFullScreenAd(_rewardedAd);
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
            _bridge.Call(kShowRewardedAd, adId);
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
                OnMediationAdClosed(FullScreenAdResult.Completed);
            }
        }

        private void OnRewardedAdLoaded() {
            if (_rewardedAd != null) {
                _rewardedAd.OnLoaded();
            } else {
                // Automatically reloaded by SDK.
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
                OnMediationAdClosed(rewarded
                    ? FullScreenAdResult.Completed
                    : FullScreenAdResult.Canceled);
            }
        }

        private void OnMediationAdClosed(FullScreenAdResult result) {
            if (_displayer.IsProcessing) {
                _displayer.Resolve(result);
                return;
            }
            Assert.IsTrue(false);
        }
    }
}