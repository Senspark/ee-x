import {
    IBannerAd,
    IFullScreenAd,
} from "../ads";
import { IPlugin } from "../core";
import { AdMobBannerAdSize } from "./AdMobBannerAdSIze";

export interface IAdMob extends IPlugin {
    initialize(): Promise<boolean>;
    getEmulatorTestDeviceHash(): string;
    addTestDevice(hash: string): void;
    createBannerAd(adId: string, adSize: AdMobBannerAdSize): IBannerAd;
    createAppOpenAd(adId: string): IFullScreenAd;
    createInterstitialAd(adId: string): IFullScreenAd;
    createRewardedInterstitialAd(adId: string): IFullScreenAd;
    createRewardedAd(adId: string): IFullScreenAd;
}