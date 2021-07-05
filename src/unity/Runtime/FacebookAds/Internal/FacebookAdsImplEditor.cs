using System.Threading.Tasks;

namespace EE.Internal {
    internal class FacebookAdsImplEditor : IFacebookAds {
        public void Destroy() {
        }

        public Task<bool> Initialize() {
            return Task.FromResult(true);
        }

        public string GetTestDeviceHash() {
            return "";
        }

        public void AddTestDevice(string hash) {
        }

        public void ClearTestDevices() {
        }

        public IBannerAd CreateBannerAd(string adId, FacebookBannerAdSize adSize) {
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