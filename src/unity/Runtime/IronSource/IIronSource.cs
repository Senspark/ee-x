using System.Threading.Tasks;

namespace EE {
    public interface IIronSource : IPlugin {
        Task<bool> Initialize(string appKey);
        IBannerAd CreateBannerAd(string adId, IronSourceBannerAdSize adSize);
        IFullScreenAd CreateInterstitialAd(string adId);
        IFullScreenAd CreateRewardedAd(string adId);
    }
}