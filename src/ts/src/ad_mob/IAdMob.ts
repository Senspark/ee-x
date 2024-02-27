import {
    IBannerAd,
    IFullScreenAd,
} from "../ads";
import { IPlugin, ILibraryAnalytics } from "../core";
import { AdMobBannerAdSize } from "./AdMobBannerAdSIze";

export interface IAdMob extends IPlugin {
    initialize(analytics: ILibraryAnalytics): Promise<boolean>;
    getEmulatorTestDeviceHash(): string;
    addTestDevice(hash: string): void;
    openTestSuite(): void;
    createBannerAd(adId: string, adSize: AdMobBannerAdSize): IBannerAd;
    createAppOpenAd(adId: string): IFullScreenAd;
    createInterstitialAd(adId: string): IFullScreenAd;
    createRewardedInterstitialAd(adId: string): IFullScreenAd;
    createRewardedAd(adId: string): IFullScreenAd;
}