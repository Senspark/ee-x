using System.Threading.Tasks;

namespace EE {
    public interface IAdMob : IPlugin {
        Task<bool> Initialize();
        string GetEmulatorTestDeviceHash();
        void AddTestDevice(string hash);
        IBannerAd CreateBannerAd(string adId, AdMobBannerAdSize adSize);
        IFullScreenAd CreateAppOpenAd(string adId);
        IFullScreenAd CreateInterstitialAd(string adId);
        IFullScreenAd CreateRewardedInterstitialAd(string adId);
        IFullScreenAd CreateRewardedAd(string adId);
    }
}