namespace EE {
    public interface IIronSource : IPlugin {
        void Initialize(string appKey);
        IInterstitialAd CreateInterstitialAd(string adId);
        IRewardedAd CreateRewardedAd(string adId);
    }
}