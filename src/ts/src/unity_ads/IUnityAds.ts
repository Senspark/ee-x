import { IFullScreenAd } from "../ads";
import { IPlugin } from "../core";

export interface IUnityAds extends IPlugin {
    initialize(gameId: string, testModeEnabled: boolean): Promise<boolean>;
    setDebugModeEnabled(enabled: boolean): void;
    createInterstitialAd(adId: string): IFullScreenAd;
    createRewardedAd(adId: string): IFullScreenAd;
}