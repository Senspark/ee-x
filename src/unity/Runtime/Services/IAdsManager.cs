using System;
using System.Threading.Tasks;

namespace EE {
    public class AdsObserver {
        public Action OnClicked { get; set; }
    }

    public interface IAdsManager : IObserverManager<AdsObserver> {
        bool IsBannerAdLoaded { get; }
        bool IsBannerAdVisible { get; set; }
        (float, float) BannerAdAnchor { get; set; }
        (float, float) BannerAdPosition { get; set; }
        (float, float) BannerAdSize { get; set; }
        bool IsRectangleAdLoaded { get; }
        bool IsRectangleAdVisible { get; set; }
        (float, float) RectangleAdAnchor { get; set; }
        (float, float) RectangleAdPosition { get; set; }
        (float, float) RectangleAdSize { get; set; }
        Task<AdResult> ShowAppOpenAd();
        Task<AdResult> ShowInterstitialAd();
        Task<AdResult> ShowRewardedAd();
    }
}