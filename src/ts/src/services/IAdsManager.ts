import {
    IBannerAd,
    IFullScreenAd,
} from "../ads";

export interface IAdsManager {
    openTestSuite(): void;
    bannerAd: IBannerAd;
    rectangleAd: IBannerAd;
    appOpenAd: IFullScreenAd;
    interstitialAd: IFullScreenAd;
    rewardedInterstitialAd: IFullScreenAd;
    rewardedAd: IFullScreenAd;
}