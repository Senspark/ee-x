import { IObserverManager } from "../core";
import { AdResult } from "./AdResult";

export interface AdsObserver {
    onClicked?(): void;
}

export interface IAdsManager extends IObserverManager<AdsObserver> {
    isBannerAdLoaded: boolean;
    isBannerAdVisible: boolean;
    bannerAdAnchor: [number, number];
    bannerAdPosition: [number, number];
    bannerAdSize: [number, number];
    isRectangleAdLoaded: boolean;
    isRectangleAdVisible: boolean;
    rectangleAdAnchor: [number, number];
    rectangleAdPosition: [number, number];
    rectangleAdSize: [number, number];
    showAppOpenAd(): Promise<AdResult>;
    showInterstitialAd(): Promise<AdResult>;
    showRewardedAd(): Promise<AdResult>;
}