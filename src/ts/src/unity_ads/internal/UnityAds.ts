import {
    FullScreenAdResult,
    IAd,
    IFullScreenAd,
} from "../../ads";
import {
    GuardedFullScreenAd,
    IAsyncHelper,
    MediationManager,
} from "../../ads/internal";
import {
    ILogger,
    IMessageBridge,
    Utils,
} from "../../core";
import { IUnityAds } from "../IUnityAds";
import { UnityInterstitialAd } from "./UnityInterstitialAd";
import { UnityRewardedAd } from "./UnityRewardedAd";

type Destroyer = () => void;

export class UnityAds implements IUnityAds {
    private readonly kPrefix = "UnityAdsBridge";
    private readonly kInitialize = this.kPrefix + "Initialize";
    private readonly kSetDebugModeEnabled = this.kPrefix + "SetDebugModeEnabled";
    private readonly kHasRewardedAd = this.kPrefix + "HasRewardedAd";
    private readonly kLoadRewardedAd = this.kPrefix + "LoadRewardedAd";
    private readonly kShowRewardedAd = this.kPrefix + "ShowRewardedAd";
    private readonly kOnLoaded = this.kPrefix + "OnLoaded";
    private readonly kOnFailedToShow = this.kPrefix + "OnFailedToShow";
    private readonly kOnClosed = this.kPrefix + "OnClosed";

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;
    private _displaying: boolean;
    private _adId?: string;
    private readonly _ads: { [index: string]: [IAd, IAd] };
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
        this._displaying = false;
        this._ads = {};
        this._displayer = MediationManager.getInstance().adDisplayer;

        this._bridge.registerHandler(this.onLoaded, this.kOnLoaded);
        this._bridge.registerHandler(message => {
            const response: {
                ad_id: string,
                message: string,
            } = JSON.parse(message);
            this.onFailedToShow(response.ad_id, response.message);
        }, this.kOnFailedToShow);
        this._bridge.registerHandler(message => {
            const response: {
                ad_id: string,
                rewarded: boolean,
            } = JSON.parse(message);
            this.onClosed(response.ad_id, response.rewarded);
        }, this.kOnClosed);
    }

    public destroy(): void {
        this._bridge.deregisterHandler(this.kOnLoaded);
        this._bridge.deregisterHandler(this.kOnFailedToShow);
        this._bridge.deregisterHandler(this.kOnClosed);
        this._destroyer();
    }

    public async initialize(gameId: string, testModeEnabled: boolean): Promise<boolean> {
        const request: {
            gameId: string,
            testModeEnabled: boolean,
        } = {
            gameId,
            testModeEnabled,
        };
        const response = await this._bridge.callAsync(this.kInitialize, JSON.stringify(request));
        return Utils.toBool(response);
    }

    public setDebugModeEnabled(enabled: boolean): void {
        this._bridge.call(this.kSetDebugModeEnabled, Utils.toString(enabled));
    }

    public createInterstitialAd(adId: string): IFullScreenAd {
        return this.createFullScreenAd(adId, () => new UnityInterstitialAd(this._displayer, this, adId));
    }

    public createRewardedAd(adId: string): IFullScreenAd {
        return this.createFullScreenAd(adId, () => new UnityRewardedAd(this._displayer, this, adId));
    }

    private createFullScreenAd(adId: string, creator: () => IFullScreenAd): IFullScreenAd {
        if (this._ads[adId] !== undefined) {
            return this._ads[adId][0] as IFullScreenAd;
        }
        const raw = creator();
        const ad = new GuardedFullScreenAd(raw);
        this._ads[adId] = [ad, raw];
        return ad;
    }

    public destroyAd(adId: string): boolean {
        if (this._ads[adId] === undefined) {
            return false;
        }
        delete this._ads[adId];
        return true;
    }

    public hasRewardedAd(adId: string): boolean {
        const response = this._bridge.call(this.kHasRewardedAd, adId);
        return Utils.toBool(response);
    }

    public async loadRewardedAd(adId: string): Promise<boolean> {
        const response = await this._bridge.callAsync(this.kLoadRewardedAd, adId);
        return Utils.toBool(response);
    }

    public showRewardedAd(adId: string): void {
        this._adId = adId;
        this._displaying = true;
        this._bridge.call(this.kShowRewardedAd, adId);
    }

    private onLoaded(adId: string): void {
        if (this._ads[adId] !== undefined) {
            const [_, ad] = this._ads[adId];
            if (ad instanceof UnityInterstitialAd) {
                ad.onLoaded();
                return;
            }
            if (ad instanceof UnityRewardedAd) {
                ad.onLoaded();
                return;
            }
        }
        // Mediation.
    }

    private onFailedToShow(adId: string, message: string): void {
        if (this._displaying) {
            // Assert.IsTrue(adId == _adId);
            this._displaying = false;
            if (this._ads[adId] !== undefined) {
                const [_, ad] = this._ads[adId];
                if (ad instanceof UnityInterstitialAd) {
                    ad.onFailedToShow(message);
                    return;
                }
                if (ad instanceof UnityRewardedAd) {
                    ad.onFailedToShow(message);
                    return;
                }
            }
        } else {
            this.onMediationAdFailedToShow(adId, message);
        }
    }

    private onClosed(adId: string, rewarded: boolean): void {
        if (this._displaying) {
            // Assert.IsTrue(adId == _adId);
            this._displaying = false;
            if (this._ads[adId] !== undefined) {
                const [_, ad] = this._ads[adId];
                if (ad instanceof UnityInterstitialAd) {
                    ad.onClosed();
                    return;
                }
                if (ad instanceof UnityRewardedAd) {
                    ad.onClosed(rewarded);
                    return;
                }
            }
        } else {
            this.onMediationAdClosed(adId, rewarded
                ? FullScreenAdResult.Completed
                : FullScreenAdResult.Canceled);
        }
    }

    private onMediationAdFailedToShow(adId: string, message: string): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(FullScreenAdResult.Failed);
            return;
        }
        // Assert.IsTrue(false);
    }

    private onMediationAdClosed(adId: string, result: FullScreenAdResult): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(result);
            return;
        }
        // Assert.IsTrue(false);
    }
}