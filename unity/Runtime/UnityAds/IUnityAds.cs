using System.Threading.Tasks;

namespace EE {
    public interface IUnityAds : IPlugin {
        Task<bool> Initialize(string gameId, bool testModeEnabled);
        void SetDebugModeEnabled(bool enabled);
        IInterstitialAd CreateInterstitialAd(string adId);
        IRewardedAd CreateRewardedAd(string adId);
    }
}