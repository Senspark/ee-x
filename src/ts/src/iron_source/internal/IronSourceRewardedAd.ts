import {
    AdObserver,
    AdResult,
    IFullScreenAd,
} from "../../ads";
import { IAsyncHelper } from "../../ads/internal";
import { ObserverManager } from "../../core";
import { IronSource } from "./IronSource";

export class IronSourceRewardedAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _displayer: IAsyncHelper<AdResult>;
    private readonly _plugin: IronSource;
    private readonly _network: string;
    private readonly _adId: string;

    public constructor(
        displayer: IAsyncHelper<AdResult>,
        plugin: IronSource,
        network: string,
        adId: string) {
        super();
        this._displayer = displayer;
        this._plugin = plugin;
        this._network = network;
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

    public show(): Promise<AdResult> {
        return this._displayer.process(
            () => this._plugin.showRewardedAd(this._adId),
            result => {
                // OK.
            });
    }

    public onLoaded(): void {
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    public onFailedToShow(code: number, message: string): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(AdResult.Failed);
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
                ? AdResult.Completed
                : AdResult.Canceled);
        } else {
            // Assert.
        }
    }
}