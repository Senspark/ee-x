using System.Threading.Tasks;

namespace EE.Internal {
    internal class AdSensparkImplEditor : IAdSenspark {
        public void Destroy() {
        }

        public Task<bool> Initialize() {
            return Task.FromResult(true);
        }

        public IBannerAd CreateBannerAd(string adId, AdSensparkBannerAdSize adSize) {
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