import {
    AdResult,
    IBannerAd,
    IFullScreenAd,
} from "../../ads";
import {
    DefaultBannerAd,
    GuardedBannerAd,
    GuardedFullScreenAd,
    IAsyncHelper,
    MediationManager,
} from "../../ads/internal";
import {
    ILogger,
    IMessageBridge,
    Utils,
} from "../../core";
import { IIronSource } from "../IIronSource";
import { IronSourceBannerAdSize } from "../IronSourceBannerAdSize";
import { IronSourceInterstitialAd } from "./IronSourceInterstitialAd";
import { IronSourceRewardedAd } from "./IronSourceRewardedAd";

type Destroyer = () => void;

export class IronSource implements IIronSource {
    private readonly kTag = `IronSource`;
    private readonly kPrefix = `IronSourceBridge`;
    private readonly kInitialize = `${this.kPrefix}Initialize`;
    private readonly kGetBannerAdSize = `${this.kPrefix}GetBannerAdSize`;
    private readonly kCreateBannerAd = `${this.kPrefix}CreateBannerAd`;
    private readonly kDestroyAd = `${this.kPrefix}DestroyAd`;
    private readonly kHasInterstitialAd = `${this.kPrefix}HasInterstitialAd`;
    private readonly kLoadInterstitialAd = `${this.kPrefix}LoadInterstitialAd`;
    private readonly kShowInterstitialAd = `${this.kPrefix}ShowInterstitialAd`;
    private readonly kHasRewardedAd = `${this.kPrefix}HasRewardedAd`;
    private readonly kShowRewardedAd = `${this.kPrefix}ShowRewardedAd`;
    private readonly kOnInterstitialAdLoaded = `${this.kPrefix}OnInterstitialAdLoaded`;
    private readonly kOnInterstitialAdFailedToLoad = `${this.kPrefix}OnInterstitialAdFailedToLoad`;
    private readonly kOnInterstitialAdFailedToShow = `${this.kPrefix}OnInterstitialAdFailedToShow`;
    private readonly kOnInterstitialAdClicked = `${this.kPrefix}OnInterstitialAdClicked`;
    private readonly kOnInterstitialAdClosed = `${this.kPrefix}OnInterstitialAdClosed`;
    private readonly kOnRewardedAdLoaded = `${this.kPrefix}OnRewardedAdLoaded`;
    private readonly kOnRewardedAdFailedToShow = `${this.kPrefix}OnRewardedAdFailedToShow`;
    private readonly kOnRewardedAdClicked = `${this.kPrefix}OnRewardedAdClicked`;
    private readonly kOnRewardedAdClosed = `${this.kPrefix}OnRewardedAdClosed`;

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;
    private readonly _network: string;
    private _bannerAd?: IBannerAd;
    private _interstitialAd?: IronSourceInterstitialAd;
    private _sharedInterstitialAd?: IFullScreenAd;
    private _rewardedAd?: IronSourceRewardedAd;
    private _sharedRewardedAd?: IFullScreenAd;
    private readonly _displayer: IAsyncHelper<AdResult>;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
        this._logger.debug(`${this.kTag}: constructor`);
        this._network = `iron_source`;
        this._displayer = MediationManager.getInstance().adDisplayer;

        this._bridge.registerHandler(_ => this.onInterstitialAdLoaded(), this.kOnInterstitialAdLoaded);
        this._bridge.registerHandler(message => {
            const json: {
                code: number,
                message: string,
            } = JSON.parse(message);
            this.onInterstitialAdFailedToLoad(json.code, json.message);
        }, this.kOnInterstitialAdFailedToLoad);
        this._bridge.registerHandler(message => {
            const json: {
                code: number,
                message: string,
            } = JSON.parse(message);
            this.onInterstitialAdFailedToShow(json.code, json.message);
        }, this.kOnInterstitialAdFailedToShow);
        this._bridge.registerHandler(_ => this.onInterstitialAdClicked(), this.kOnInterstitialAdClicked);
        this._bridge.registerHandler(_ => this.onInterstitialAdClosed(), this.kOnInterstitialAdClosed);
        this._bridge.registerHandler(_ => this.onRewardedAdLoaded(), this.kOnRewardedAdLoaded);
        this._bridge.registerHandler(message => {
            const json: {
                code: number,
                message: string,
            } = JSON.parse(message);
            this.onRewardedAdFailedToShow(json.code, json.message);
        }, this.kOnRewardedAdFailedToShow);
        this._bridge.registerHandler(_ => this.onRewardedAdClicked(), this.kOnRewardedAdClicked);
        this._bridge.registerHandler(message => this.onRewardedAdClosed(Utils.toBool(message)), this.kOnRewardedAdClosed);
    }

    public destroy(): void {
        this._logger.debug(`${this.kTag}: destroy`);
        this._bridge.deregisterHandler(this.kOnInterstitialAdLoaded);
        this._bridge.deregisterHandler(this.kOnInterstitialAdFailedToLoad);
        this._bridge.deregisterHandler(this.kOnInterstitialAdFailedToShow);
        this._bridge.deregisterHandler(this.kOnInterstitialAdClicked);
        this._bridge.deregisterHandler(this.kOnInterstitialAdClosed);
        this._bridge.deregisterHandler(this.kOnRewardedAdLoaded);
        this._bridge.deregisterHandler(this.kOnRewardedAdFailedToShow);
        this._bridge.deregisterHandler(this.kOnRewardedAdClicked);
        this._bridge.deregisterHandler(this.kOnRewardedAdClosed);
        this._destroyer();
    }

    public async initialize(appKey: string): Promise<boolean> {
        const response = await this._bridge.callAsync(this.kInitialize, appKey);
        return Utils.toBool(response);
    }

    private getBannerAdSize(adSize: IronSourceBannerAdSize): [number, number] {
        const response = this._bridge.call(this.kGetBannerAdSize, `${adSize}`);
        const json: {
            width: number,
            height: number,
        } = JSON.parse(response);
        return [json.width, json.height];
    }

    public createBannerAd(adId: string, adSize: IronSourceBannerAdSize): IBannerAd {
        this._logger.debug(`${this.kTag}: createBannerAd: id = ${adId} size = ${adSize}`);
        if (this._bannerAd !== undefined) {
            return this._bannerAd;
        }
        const request = {
            [`adId`]: adId,
            [`adSize`]: parseInt(`${adSize}`),
        };
        const response = this._bridge.call(this.kCreateBannerAd, JSON.stringify(request));
        if (!Utils.toBool(response)) {
            throw new Error(`Failed to create banner ad: ${adId}`);
        }
        const size = this.getBannerAdSize(adSize);
        this._bannerAd = new GuardedBannerAd(
            new DefaultBannerAd("IronSourceBannerAd", this._bridge, this._logger,
                () => this.destroyBannerAd(adId),
                this._network, adId, size));
        return this._bannerAd;
    }

    public destroyBannerAd(adId: string): boolean {
        this._logger.debug(`${this.kTag}: destroyBannerAd: id = ${adId}`);
        if (this._bannerAd === undefined) {
            return false;
        }
        const response = this._bridge.call(this.kDestroyAd, adId);
        if (!Utils.toBool(response)) {
            // Assert.
            return false;
        }
        this._bannerAd = undefined;
        return true;
    }

    public createInterstitialAd(adId: string): IFullScreenAd {
        this._logger.debug(`${this.kTag}: createInterstitialAd: id = ${adId}`);
        if (this._sharedInterstitialAd !== undefined) {
            return this._sharedInterstitialAd;
        }
        this._interstitialAd = new IronSourceInterstitialAd(
            this._displayer, this, this._network, adId);
        this._sharedInterstitialAd = new GuardedFullScreenAd(this._interstitialAd);
        return this._sharedInterstitialAd;
    }

    public destroyInterstitialAd(adId: string): boolean {
        this._logger.debug(`${this.kTag}: destroyInterstitialAd: id = ${adId}`);
        if (this._sharedInterstitialAd == undefined) {
            return false;
        }
        this._interstitialAd = undefined;
        this._sharedInterstitialAd = undefined;
        return true;
    }

    public createRewardedAd(adId: string): IFullScreenAd {
        this._logger.debug(`${this.kTag}: createRewardedAd: id = ${adId}`);
        if (this._sharedRewardedAd !== undefined) {
            return this._sharedRewardedAd;
        }
        this._rewardedAd = new IronSourceRewardedAd(
            this._displayer, this, this._network, adId);
        this._sharedRewardedAd = new GuardedFullScreenAd(this._rewardedAd);
        return this._sharedRewardedAd;
    }

    public destroyRewardedAd(adId: string): boolean {
        this._logger.debug(`${this.kTag}: destroyRewardedAd: id = ${adId}`);
        if (this._sharedRewardedAd == undefined) {
            return false;
        }
        this._rewardedAd = undefined;
        this._sharedRewardedAd = undefined;
        return true;
    }

    public hasInterstitialAd(): boolean {
        const response = this._bridge.call(this.kHasInterstitialAd);
        return Utils.toBool(response);
    }

    public loadInterstitialAd(): void {
        this._bridge.call(this.kLoadInterstitialAd);
    }

    public showInterstitialAd(adId: string): void {
        this._bridge.call(this.kShowInterstitialAd, adId);
    }

    public hasRewardedAd(): boolean {
        const response = this._bridge.call(this.kHasRewardedAd);
        return Utils.toBool(response);
    }

    public showRewardedAd(adId: string): void {
        this._bridge.call(this.kShowRewardedAd, adId);
    }

    private onInterstitialAdLoaded(): void {
        if (this._interstitialAd !== undefined) {
            this._interstitialAd.onLoaded();
        } else {
            // Assert.
        }
    }

    private onInterstitialAdFailedToLoad(code: number, message: string): void {
        if (this._interstitialAd !== undefined) {
            this._interstitialAd.onFailedToLoad(code, message);
        } else {
            // Assert.
        }
    }

    private onInterstitialAdFailedToShow(code: number, message: string): void {
        if (this._interstitialAd !== undefined) {
            this._interstitialAd.onFailedToShow(code, message);
        } else {
            // Assert.
        }
    }

    private onInterstitialAdClicked(): void {
        if (this._interstitialAd !== undefined) {
            this._interstitialAd.onClicked();
        } else {
            // Assert.
        }
    }

    private onInterstitialAdClosed(): void {
        if (this._interstitialAd !== undefined) {
            this._interstitialAd.onClosed();
        } else {
            this.onMediationAdClosed(AdResult.Completed);
        }
    }

    private onRewardedAdLoaded(): void {
        if (this._rewardedAd !== undefined) {
            this._rewardedAd.onLoaded();
        } else {
            // Automatically reloaded by SDK.
        }
    }

    private onRewardedAdFailedToShow(code: number, message: string): void {
        if (this._rewardedAd !== undefined) {
            this._rewardedAd.onFailedToShow(code, message);
        } else {
            // Assert.
        }
    }

    private onRewardedAdClicked(): void {
        if (this._rewardedAd !== undefined) {
            this._rewardedAd.onClicked();
        } else {
            // Assert.
        }
    }

    private onRewardedAdClosed(rewarded: boolean): void {
        if (this._rewardedAd !== undefined) {
            this._rewardedAd.onClosed(rewarded);
        } else {
            this.onMediationAdClosed(rewarded
                ? AdResult.Completed
                : AdResult.Canceled);
        }
    }

    private onMediationAdClosed(result: AdResult): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(result);
            return;
        }
        // Assert.
    }
}