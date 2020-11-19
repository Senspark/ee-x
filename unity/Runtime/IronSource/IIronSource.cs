using System.Threading.Tasks;

namespace EE {
    public interface IIronSource : IPlugin {
        Task<bool> Initialize(string appKey);
        IInterstitialAd CreateInterstitialAd(string adId);
        IRewardedAd CreateRewardedAd(string adId);
    }
}