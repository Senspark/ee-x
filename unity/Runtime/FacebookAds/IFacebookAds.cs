using System.Threading.Tasks;

namespace EE {
    public interface IFacebookAds : IPlugin {
        Task<bool> Initialize();
        string GetTestDeviceHash();
        void AddTestDevice(string hash);
        void ClearTestDevices();
        IAdView CreateBannerAd(string adId, FacebookBannerAdSize adSize);
        IInterstitialAd CreateInterstitialAd(string ad);
        IRewardedAd CreateRewardedAd(string adId);
    }
}