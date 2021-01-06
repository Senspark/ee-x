using System.Threading.Tasks;

namespace EE {
    public enum AdResult {
        NotInitialized,
        NotConfigured,
        Capped,
        NoInternet,
        NotLoaded,
        Failed,
        Canceled,
        Completed
    }

    public interface IAdsManager {
        bool IsBannerAdVisible { get; set; }
        (float, float) BannerAdAnchor { get; set; }
        (float, float) BannerAdPosition { get; set; }
        (float, float) BannerAdSize { get; set; }
        Task<AdResult> ShowAppOpenAd();
        Task<AdResult> ShowInterstitialAd();
        Task<AdResult> ShowRewardedAd();
    }
}