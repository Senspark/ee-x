import {
    IBannerAd,
    IFullScreenAd,
} from "../ads";
import {
    Capper,
    ICapper,
} from "../ads/internal";
import { CocosBannerAd } from "../cocos";
import {
    IAdsManager,
} from "./IAdsManager";
import {
    AdFormat,
    AdsConfig,
    GenericAd,
    LazyBannerAd,
    LazyFullScreenAd,
    Network,
} from "./internal";

export class DefaultAdsManager implements IAdsManager {
    private _initializer?: Promise<boolean>;
    private _initialized: boolean;
    private readonly _displayCapper: ICapper;
    private readonly _config: AdsConfig;
    private readonly _bannerAds: { [key: string]: LazyBannerAd };
    private readonly _fullScreenAds: { [key: string]: LazyFullScreenAd };

    public constructor(configJson: string) {
        this._initialized = false;
        this._displayCapper = new Capper(0.1);
        this._config = AdsConfig.parse(configJson);
        this._bannerAds = {};
        this._fullScreenAds = {};
    }

    public initialize(): Promise<boolean> {
        return this._initializer ?? (this._initializer = this.initializeImpl());
    }

    private async initializeImpl(): Promise<boolean> {
        this._bannerAds[AdFormat.Banner] = new LazyBannerAd();
        this._bannerAds[AdFormat.Rectangle] = new LazyBannerAd();
        this._fullScreenAds[AdFormat.AppOpen] = new LazyFullScreenAd(this._displayCapper);
        this._fullScreenAds[AdFormat.Interstitial] = new LazyFullScreenAd(this._displayCapper);
        this._fullScreenAds[AdFormat.RewardedInterstitial] = new LazyFullScreenAd(this._displayCapper);
        this._fullScreenAds[AdFormat.Rewarded] = new LazyFullScreenAd(this._displayCapper);
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
    }

    private initializeFullScreenAd(format: AdFormat) {
        const ad = this._config.createAd(format);
        if (ad instanceof GenericAd) {
            // OK.
        } else {
            return;
        }
        this._fullScreenAds[format].ad = ad;
    }

    public openTestSuite(): void {
        this._config.openTestSuite(Network.AdMob);
    }

    public get bannerAd(): IBannerAd {
        return this._bannerAds[AdFormat.Banner];
    }

    public get rectangleAd(): IBannerAd {
        return this._bannerAds[AdFormat.Rectangle];
    }

    public get appOpenAd(): IFullScreenAd {
        return this._fullScreenAds[AdFormat.AppOpen];
    }

    public get interstitialAd(): IFullScreenAd {
        return this._fullScreenAds[AdFormat.Interstitial];
    }

    public get rewardedInterstitialAd(): IFullScreenAd {
        return this._fullScreenAds[AdFormat.RewardedInterstitial];
    }

    public get rewardedAd(): IFullScreenAd {
        return this._fullScreenAds[AdFormat.Rewarded];
    }
}