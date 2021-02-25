namespace EE {
    [Service("EE" + nameof(IAdsManager))]
    public interface IAdsManager : IService {
        void OpenTestSuite();
        IBannerAd BannerAd { get; }
        IBannerAd RectangleAd { get; }
        IFullScreenAd AppOpenAd { get; }
        IFullScreenAd InterstitialAd { get; }
        IFullScreenAd RewardedInterstitialAd { get; }
        IFullScreenAd RewardedAd { get; }
    }
}