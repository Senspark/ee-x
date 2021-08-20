using System.Threading.Tasks;

namespace EE {
    public interface IAdSenspark : IPlugin {
        Task<bool> Initialize();
        IBannerAd CreateBannerAd(string adId, AdSensparkBannerAdSize adSize);
        IFullScreenAd CreateAppOpenAd(string adId);
        IFullScreenAd CreateInterstitialAd(string adId);
        IFullScreenAd CreateRewardedInterstitialAd(string adId);
        IFullScreenAd CreateRewardedAd(string adId);
    }
}