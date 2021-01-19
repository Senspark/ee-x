import {
    AdObserver,
    FullScreenAdResult,
    IFullScreenAd,
} from "../../ads";
import { IAsyncHelper } from "../../ads/internal";
import { ObserverManager } from "../../core";
import { IronSource } from "./IronSource";

export class IronSourceRewardedAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;
    private readonly _plugin: IronSource;
    private readonly _adId: string;

    public constructor(
        displayer: IAsyncHelper<FullScreenAdResult>, plugin: IronSource, adId: string) {
        super();
        this._displayer = displayer;
        this._plugin = plugin;
        this._adId = adId;
    }

    public destroy(): void {
        this._plugin.destroyRewardedAd(this._adId);
    }

    public get isLoaded(): boolean {
        return this._plugin.hasRewardedAd();
    }

    public async load(): Promise<boolean> {
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

    public onClicked(): void {
        this.dispatchEvent(observer => observer.onClicked && observer.onClicked());
    }

    public onClosed(rewarded: boolean): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(rewarded
                ? FullScreenAdResult.Completed
                : FullScreenAdResult.Canceled);
        } else {
            // Assert.
        }
    }
}