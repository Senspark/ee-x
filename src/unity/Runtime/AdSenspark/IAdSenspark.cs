using System.Threading.Tasks;

using Jsonite;

namespace EE {
    internal interface IAdSenspark : IPlugin {
        Task<bool> Initialize(string url);
        IBannerAd CreateBannerAd(string adId, AdSensparkBannerAdSize adSize, JsonObject node);
        IFullScreenAd CreateAppOpenAd(string adId, JsonObject node);
        IFullScreenAd CreateInterstitialAd(string adId, JsonObject node);
        IFullScreenAd CreateRewardedInterstitialAd(string adId, JsonObject node);
        IFullScreenAd CreateRewardedAd(string adId, JsonObject node);
    }
}