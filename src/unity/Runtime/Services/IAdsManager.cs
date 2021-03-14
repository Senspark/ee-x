using System.Collections.Generic;

using EE.Internal;

using UnityEngine.Scripting;

namespace EE {
    public class AdEvent : IAnalyticsEvent {
        public string EventName => AnalyticsUtils.MakeLibraryEvent("ad");

        public Dictionary<string, object> Parameters => new Dictionary<string, object> {
            ["format"] = AnalyticsUtils.ParseAdFormat(Format),
            ["result"] = AnalyticsUtils.ParseAdResult(Result)
        };

        public AdFormat Format;
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