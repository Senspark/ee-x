namespace EE {
    public interface IUnityAds : IPlugin {
        void Initialize(string gameId, bool testModeEnabled);
        void SetDebugModeEnabled(bool enabled);
        IInterstitialAd CreateInterstitialAd(string adId);
        IRewardedAd CreateRewardedAd(string adId);
    }
}