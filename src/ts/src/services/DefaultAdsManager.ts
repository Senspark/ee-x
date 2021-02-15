import { CocosBannerAd } from "../cocos";
import {
    ObserverHandle,
    ObserverManager,
    Utils,
} from "../core";
import { AdResult } from "./AdResult";
import {
    AdsObserver,
    IAdsManager,
} from "./IAdsManager";
import {
    AdFormat,
    AdsConfig,
    GenericAd,
    LazyBannerAd,
} from "./internal";

export class DefaultAdsManager extends ObserverManager<AdsObserver> implements IAdsManager {
    private _initializer?: Promise<boolean>;
    private _initialized: boolean;
    private _fullScreenAdCapped: boolean;
    private readonly _config: AdsConfig;
    private readonly _bannerAds: { [key: string]: LazyBannerAd };
    private readonly _fullScreenAds: { [key: string]: GenericAd };
    private readonly _handle: ObserverHandle;

    public constructor(configJson: string) {
        super();
        this._initialized = false;
        this._fullScreenAdCapped = false;
        this._config = AdsConfig.parse(configJson);
        this._bannerAds = {};
        this._fullScreenAds = {};
        this._handle = new ObserverHandle();
    }

    public initialize(): Promise<boolean> {
        return this._initializer ?? (this._initializer = this.initializeImpl());
    }

    private async initializeImpl(): Promise<boolean> {
        this._bannerAds[AdFormat.Banner] = new LazyBannerAd();
        this._bannerAds[AdFormat.Rectangle] = new LazyBannerAd();
        await this._config.initialize();
        this.initializeBannerAd(AdFormat.Banner);
        this.initializeBannerAd(AdFormat.Rectangle);
        this.initializeFullScreenAd(AdFormat.AppOpen);
        this.initializeFullScreenAd(AdFormat.Interstitial);
        this.initializeFullScreenAd(AdFormat.RewardedInterstitial);
        this.initializeFullScreenAd(AdFormat.Rewarded);
        this._initialized = true;
        return true;
    }

    private initializeBannerAd(format: AdFormat): void {
        const ad = this._config.createAd(format);
        if (ad instanceof CocosBannerAd) {
            // OK.
        } else {
            return;
        }
        this._bannerAds[format].ad = ad;
        this._handle.bind(this._bannerAds[format])
            .addObserver({
                onClicked: () => this.dispatchEvent(observer => observer.onClicked && observer.onClicked()),
            });
    }

    private initializeFullScreenAd(format: AdFormat) {
        const ad = this._config.createAd(format);
        if (ad instanceof GenericAd) {
            // OK.
        } else {
            return;
        }
        this._fullScreenAds[format] = ad;
        this._handle.bind(ad)
            .addObserver({
                onClicked: () => this.dispatchEvent(observer => observer.onClicked && observer.onClicked()),
            });
    }

    public get isBannerAdLoaded(): boolean {
        return this._bannerAds[AdFormat.Banner].isLoaded;
    }

    public get isBannerAdVisible(): boolean {
        return this._bannerAds[AdFormat.Banner].isVisible;
    }

    public set isBannerAdVisible(value: boolean) {
        this._bannerAds[AdFormat.Banner].isVisible = value;
    }

    public get bannerAdAnchor(): [number, number] {
        return this._bannerAds[AdFormat.Banner].anchor;
    }

    public set bannerAdAnchor(value: [number, number]) {
        this._bannerAds[AdFormat.Banner].anchor = value;
    }

    public get bannerAdPosition(): [number, number] {
        return this._bannerAds[AdFormat.Banner].position;
    }

    public set bannerAdPosition(value: [number, number]) {
        this._bannerAds[AdFormat.Banner].position = value;
    }

    public get bannerAdSize(): [number, number] {
        return this._bannerAds[AdFormat.Banner].size;
    }

    public set bannerAdSize(value: [number, number]) {
        this._bannerAds[AdFormat.Banner].size = value;
    }

    public get isRectangleAdLoaded(): boolean {
        return this._bannerAds[AdFormat.Rectangle].isLoaded;
    }

    public set isRectangleAdVisible(value: boolean) {
        this._bannerAds[AdFormat.Rectangle].isVisible = value;
    }

    public get rectangleAdAnchor(): [number, number] {
        return this._bannerAds[AdFormat.Rectangle].anchor;
    }

    public set rectangleAdAnchor(value: [number, number]) {
        this._bannerAds[AdFormat.Rectangle].anchor = value;
    }

    public get rectangleAdPosition(): [number, number] {
        return this._bannerAds[AdFormat.Rectangle].position;
    }

    public set rectangleAdPosition(value: [number, number]) {
        this._bannerAds[AdFormat.Rectangle].position = value;
    }

    public get rectangleAdSize(): [number, number] {
        return this._bannerAds[AdFormat.Rectangle].size;
    }

    public set rectangleAdSize(value: [number, number]) {
        this._bannerAds[AdFormat.Rectangle].size = value;
    }

    public async showAppOpenAd(): Promise<AdResult> {
        return await this.showFullScreenAd(AdFormat.AppOpen);
    }

    public async showInterstitialAd(): Promise<AdResult> {
        return await this.showFullScreenAd(AdFormat.Interstitial);
    }

    public async showRewardedInterstitialAd(): Promise<AdResult> {
        return await this.showFullScreenAd(AdFormat.RewardedInterstitial);
    }

    public async showRewardedAd(): Promise<AdResult> {
        return await this.showFullScreenAd(AdFormat.Rewarded);
    }

    private async showFullScreenAd(format: AdFormat): Promise<AdResult> {
        if (!this._initialized) {
            return AdResult.NotInitialized;
        }
        const ad = this._fullScreenAds[format];
        if (ad !== undefined) {
            // OK.
        } else {
            return AdResult.NotConfigured;
        }
        if (this._fullScreenAdCapped) {
            return AdResult.Capped;
        }
        this._fullScreenAdCapped = true;
        var result = await ad.show();
        Utils.noAwait(async () => {
            await Utils.delay(0.1);
            this._fullScreenAdCapped = false;
        });
        return result;
    }
}