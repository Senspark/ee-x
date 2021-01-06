using System.Threading.Tasks;

namespace EE {
    public interface IAdMob : IPlugin {
        Task<bool> Initialize();
        string GetEmulatorTestDeviceHash();
        void AddTestDevice(string hash);
        IAdView CreateBannerAd(string adId, AdMobBannerAdSize adSize);
        IInterstitialAd CreateInterstitialAd(string adId);
        IRewardedAd CreateRewardedAd(string adId);
        IInterstitialAd CreateAppOpenAd(string adId);
    }
}