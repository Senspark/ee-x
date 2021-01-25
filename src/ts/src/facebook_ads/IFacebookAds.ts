import {
    IBannerAd,
    IFullScreenAd,
} from "../ads";
import { IPlugin } from "../core";
import { FacebookBannerAdSize } from "./FacebookBannerAdSize";

export interface IFacebookAds extends IPlugin {
    initialize(): Promise<boolean>;
    getTestDeviceHash(): string;
    addTestDevice(hash: string): void;
    clearTestDevices(): void;
    createBannerAd(adId: string, adSize: FacebookBannerAdSize): IBannerAd;
    createInterstitialAd(adId: string): IFullScreenAd;
    createRewardedAd(adId: string): IFullScreenAd;
}