import {
    FullScreenAdResult,
    IFullScreenAd,
} from "../../ads";
import {
    GuardedFullScreenAd,
    IAsyncHelper,
    MediationManager,
} from "../../ads/internal";
import {
    IMessageBridge,
    Utils,
} from "../../core";
import { IIronSource } from "../IIronSource";
import { IronSourceInterstitialAd } from "./IronSourceInterstitialAd";
import { IronSourceRewardedAd } from "./IronSourceRewardedAd";

export class IronSource implements IIronSource {
    private readonly kPrefix = "IronSourceBridge";
    private readonly kInitialize = this.kPrefix + "Initialize";
    private readonly kHasInterstitialAd = this.kPrefix + "HasInterstitialAd";
    private readonly kLoadInterstitialAd = this.kPrefix + "LoadInterstitialAd";
    private readonly kShowInterstitialAd = this.kPrefix + "ShowInterstitialAd";
    private readonly kHasRewardedAd = this.kPrefix + "HasRewardedAd";
    private readonly kShowRewardedAd = this.kPrefix + "ShowRewardedAd";
    private readonly kOnInterstitialAdLoaded = this.kPrefix + "OnInterstitialAdLoaded";
    private readonly kOnInterstitialAdFailedToLoad = this.kPrefix + "OnInterstitialAdFailedToLoad";
    private readonly kOnInterstitialAdFailedToShow = this.kPrefix + "OnInterstitialAdFailedToShow";
    private readonly kOnInterstitialAdClicked = this.kPrefix + "OnInterstitialAdClicked";
    private readonly kOnInterstitialAdClosed = this.kPrefix + "OnInterstitialAdClosed";
    private readonly kOnRewardedAdLoaded = this.kPrefix + "OnRewardedAdLoaded";
    private readonly kOnRewardedAdFailedToShow = this.kPrefix + "OnRewardedAdFailedToShow";
    private readonly kOnRewardedAdClicked = this.kPrefix + "OnRewardedAdClicked";
    private readonly kOnRewardedAdClosed = this.kPrefix + "OnRewardedAdClosed";

    private readonly _bridge: IMessageBridge;
    private _interstitialAd?: IronSourceInterstitialAd;
    private _sharedInterstitialAd?: IFullScreenAd;
    private _rewardedAd?: IronSourceRewardedAd;
    private _sharedRewardedAd?: IFullScreenAd;
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;

    public constructor(bridge: IMessageBridge) {
        this._bridge = bridge;
        this._displayer = MediationManager.getInstance().adDisplayer;

        this._bridge.registerHandler(_ => this.onInterstitialAdLoaded(), this.kOnInterstitialAdLoaded);
        this._bridge.registerHandler(this.onInterstitialAdFailedToLoad, this.kOnInterstitialAdFailedToLoad);
        this._bridge.registerHandler(this.onInterstitialAdFailedToShow, this.kOnInterstitialAdFailedToShow);
        this._bridge.registerHandler(_ => this.onInterstitialAdClicked(), this.kOnInterstitialAdClicked);
        this._bridge.registerHandler(_ => this.onInterstitialAdClosed(), this.kOnInterstitialAdClosed);
        this._bridge.registerHandler(_ => this.onRewardedAdLoaded(), this.kOnRewardedAdLoaded);
        this._bridge.registerHandler(this.onRewardedAdFailedToShow, this.kOnRewardedAdFailedToShow);
        this._bridge.registerHandler(_ => this.onRewardedAdClicked(), this.kOnRewardedAdClicked);
        this._bridge.registerHandler(message => this.onRewardedAdClosed(Utils.toBool(message)), this.kOnRewardedAdClosed);
    }

    public destroy(): void {
        this._bridge.deregisterHandler(this.kOnInterstitialAdLoaded);
        this._bridge.deregisterHandler(this.kOnInterstitialAdFailedToLoad);
        this._bridge.deregisterHandler(this.kOnInterstitialAdFailedToShow);
        this._bridge.deregisterHandler(this.kOnInterstitialAdClicked);
        this._bridge.deregisterHandler(this.kOnInterstitialAdClosed);
        this._bridge.deregisterHandler(this.kOnRewardedAdLoaded);
        this._bridge.deregisterHandler(this.kOnRewardedAdFailedToShow);
        this._bridge.deregisterHandler(this.kOnRewardedAdClicked);
        this._bridge.deregisterHandler(this.kOnRewardedAdClosed);
    }

    public async initialize(appKey: string): Promise<boolean> {
        const response = await this._bridge.callAsync(this.kInitialize, appKey);
        return Utils.toBool(response);
    }

    public createInterstitialAd(adId: string): IFullScreenAd {
        if (this._sharedInterstitialAd != null) {
            return this._sharedInterstitialAd;
        }
        this._interstitialAd = new IronSourceInterstitialAd(this._displayer, this, adId);
        this._sharedInterstitialAd = new GuardedFullScreenAd(this._interstitialAd);
        return this._sharedInterstitialAd;
    }

    public destroyInterstitialAd(adId: string): boolean {
        if (this._sharedInterstitialAd == undefined) {
            return false;
        }
        this._interstitialAd = undefined;
        this._sharedInterstitialAd = undefined;
        return true;
    }

    public createRewardedAd(adId: string): IFullScreenAd {
        if (this._sharedRewardedAd != undefined) {
            return this._sharedRewardedAd;
        }
        this._rewardedAd = new IronSourceRewardedAd(this._displayer, this, adId);
        this._sharedRewardedAd = new GuardedFullScreenAd(this._rewardedAd);
        return this._sharedRewardedAd;
    }

    public destroyRewardedAd(adId: string): boolean {
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
        if (this._interstitialAd != undefined) {
            this._interstitialAd.onLoaded();
        } else {
            // Assert.
        }
    }

    private onInterstitialAdFailedToLoad(message: string): void {
        if (this._interstitialAd != undefined) {
            this._interstitialAd.onFailedToLoad(message);
        } else {
            // Assert.
        }
    }

    private onInterstitialAdFailedToShow(message: string): void {
        if (this._interstitialAd != undefined) {
            this._interstitialAd.onFailedToShow(message);
        } else {
            // Assert.
        }
    }

    private onInterstitialAdClicked(): void {
        if (this._interstitialAd != undefined) {
            this._interstitialAd.onClicked();
        } else {
            // Assert.
        }
    }

    private onInterstitialAdClosed(): void {
        if (this._interstitialAd != undefined) {
            this._interstitialAd.onClosed();
        } else {
            this.onMediationAdClosed(FullScreenAdResult.Completed);
        }
    }

    private onRewardedAdLoaded(): void {
        if (this._rewardedAd != undefined) {
            this._rewardedAd.onLoaded();
        } else {
            // Automatically reloaded by SDK.
        }
    }

    private onRewardedAdFailedToShow(message: string): void {
        if (this._rewardedAd != undefined) {
            this._rewardedAd.onFailedToShow(message);
        } else {
            // Assert.
        }
    }

    private onRewardedAdClicked(): void {
        if (this._rewardedAd != undefined) {
            this._rewardedAd.onClicked();
        } else {
            // Assert.
        }
    }

    private onRewardedAdClosed(rewarded: boolean): void {
        if (this._rewardedAd != undefined) {
            this._rewardedAd.onClosed(rewarded);
        } else {
            this.onMediationAdClosed(rewarded
                ? FullScreenAdResult.Completed
                : FullScreenAdResult.Canceled);
        }
    }

    private onMediationAdClosed(result: FullScreenAdResult): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(result);
            return;
        }
        // Assert.
    }
}