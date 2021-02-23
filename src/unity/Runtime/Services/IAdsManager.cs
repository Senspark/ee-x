using System;
using System.Threading.Tasks;

namespace EE {
    public interface IAdsManager {
        IBannerAd BannerAd { get; }
        IBannerAd RectangleAd { get; }
        IFullScreenAd AppOpenAd { get; }
        IFullScreenAd InterstitialAd { get; }
        IFullScreenAd RewardedInterstitialAd { get; }
        IFullScreenAd RewardedAd { get; }
    }
}