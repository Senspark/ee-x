using System.Threading.Tasks;

namespace EE.Internal {
    internal class UnityAdsImplEditor : IUnityAds {
        public void Destroy() {
        }

        public Task<bool> Initialize(string gameId, bool testModeEnabled) {
            return Task.FromResult(true);
        }

        public void SetDebugModeEnabled(bool enabled) {
        }

        public IBannerAd CreateBannerAd(string adId, UnityBannerAdSize adSize) {
            return new NullBannerAd();
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return new NullFullScreenAd();
        }
    }
}