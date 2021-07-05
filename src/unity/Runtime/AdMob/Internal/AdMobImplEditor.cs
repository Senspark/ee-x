using System.Threading.Tasks;

namespace EE.Internal {
    internal class AdMobImplEditor : IAdMob {
        public void Destroy() {
        }

        public Task<bool> Initialize() {
            return Task.FromResult(true);
        }

        public string GetEmulatorTestDeviceHash() {
            return "";
        }

        public void AddTestDevice(string hash) {
        }

        public void OpenTestSuite() {
        }

        public IBannerAd CreateBannerAd(string adId, AdMobBannerAdSize adSize) {
            return new NullBannerAd();
        }

        public IFullScreenAd CreateAppOpenAd(string adId) {
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateInterstitialAd(string adId) {
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateRewardedInterstitialAd(string adId) {
            return new NullFullScreenAd();
        }

        public IFullScreenAd CreateRewardedAd(string adId) {
            return new NullFullScreenAd();
        }
    }
}