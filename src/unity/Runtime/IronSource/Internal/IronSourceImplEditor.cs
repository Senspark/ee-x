using System.Threading.Tasks;

namespace EE.Internal {
    internal class IronSourceImplEditor : IIronSource {
        public void Destroy() {
        }

        public Task<bool> Initialize(string appKey) {
            return Task.FromResult(true);
        }

        public IBannerAd CreateBannerAd(string adId, IronSourceBannerAdSize adSize) {
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