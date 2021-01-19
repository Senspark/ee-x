import { IFullScreenAd } from "../ads";
import { IPlugin } from "../core";

export interface IIronSource extends IPlugin {
    initialize(appKey: string): Promise<boolean>;
    createInterstitialAd(adId: string): IFullScreenAd;
    createRewardedAd(adId: string): IFullScreenAd;
}