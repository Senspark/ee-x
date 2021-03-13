using UnityEngine.Scripting;

namespace EE {
    public class AdEvent : IAnalyticsEvent {
        public string EventName => "ee_ad_event";

        [AnalyticsParameter("format")]
        public AdFormat Format;

        [AnalyticsParameter("result")]
        public AdResult Result;
    }

    [Preserve]
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