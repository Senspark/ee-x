import {
    IBannerAd,
    IFullScreenAd,
} from "../ads";
import { IPlugin } from "../core";
import { IronSourceBannerAdSize } from "./IronSourceBannerAdSize";

export interface IIronSource extends IPlugin {
    initialize(appKey: string): Promise<boolean>;
    createBannerAd(adId: string, adSize: IronSourceBannerAdSize): IBannerAd;
    createInterstitialAd(adId: string): IFullScreenAd;
    createRewardedAd(adId: string): IFullScreenAd;
}