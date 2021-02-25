namespace EE {
    public interface IAdsManager {
        void OpenTestSuite();
        IBannerAd BannerAd { get; }
        IBannerAd RectangleAd { get; }
        IFullScreenAd AppOpenAd { get; }
        IFullScreenAd InterstitialAd { get; }
        IFullScreenAd RewardedInterstitialAd { get; }
        IFullScreenAd RewardedAd { get; }
    }
}