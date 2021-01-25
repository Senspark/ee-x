import {
    AdObserver,
    FullScreenAdResult,
    IFullScreenAd,
} from "../../ads";
import { IAsyncHelper } from "../../ads/internal";
import {
    ObserverManager,
    Utils,
} from "../../core";
import { UnityAds } from "./UnityAds";

export class UnityInterstitialAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;
    private readonly _plugin: UnityAds;
    private readonly _adId: string;

    public constructor(
        displayer: IAsyncHelper<FullScreenAdResult>, plugin: UnityAds, adId: string) {
        super();
        this._displayer = displayer;
        this._plugin = plugin;
        this._adId = adId;
    }

    public destroy(): void {
        this._plugin.destroyAd(this._adId);
    }

    public get isLoaded(): boolean {
        return this._plugin.hasRewardedAd(this._adId);
    }

    public async load(): Promise<boolean> {
        Utils.noAwait(async () => { //
            await this._plugin.loadRewardedAd(this._adId);
        });
        return this.isLoaded;
    }

    public show(): Promise<FullScreenAdResult> {
        return this._displayer.process(
            () => this._plugin.showRewardedAd(this._adId),
            result => {
                // OK.
            });
    }

    public onLoaded(): void {
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    public onFailedToShow(message: string): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(FullScreenAdResult.Failed);
        } else {
            // Assert.
        }
    }

    public onClosed(): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(FullScreenAdResult.Completed);
        } else {
            // Assert.
        }
    }
}