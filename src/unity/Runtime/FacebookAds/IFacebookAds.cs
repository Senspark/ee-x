using System.Threading.Tasks;

namespace EE {
    public interface IFacebookAds : IPlugin {
        Task<bool> Initialize();
        string GetTestDeviceHash();
        void AddTestDevice(string hash);
        void ClearTestDevices();
        IAdView CreateBannerAd(string adId, FacebookBannerAdSize adSize);
        IFullScreenAd CreateInterstitialAd(string ad);
        IFullScreenAd CreateRewardedAd(string adId);
    }
}