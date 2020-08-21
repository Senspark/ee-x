namespace EE {
    public interface IFacebookAds : IPlugin {
        string GetTestDeviceHash();
        void AddTestDevice(string hash);
        void ClearTestDevices();
        IAdView CreateBannerAd(string adId, FacebookBannerAdSize adSize);
        IInterstitialAd CreateInterstitialAd(string ad);
        IRewardedAd CreateRewardedAd(string adId);
    }
}