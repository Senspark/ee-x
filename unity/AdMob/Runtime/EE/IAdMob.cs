namespace EE {
    public interface IAdMob : IPlugin {
        void Initialize();
        string GetEmulatorTestDeviceHash();
        void AddTestDevice(string hash);
        IAdView CreateBannerAd(string adId, AdMobBannerAdSize adSize);
        IInterstitialAd CreateInterstitialAd(string adId);
        IRewardedAd CreateRewardedAd(string adId);
    }
}