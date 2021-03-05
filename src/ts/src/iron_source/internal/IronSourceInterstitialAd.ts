import {
    AdObserver,
    AdResult,
    IFullScreenAd,
} from "../../ads";
import {
    AsyncHelper,
    IAsyncHelper,
} from "../../ads/internal";
import { ObserverManager } from "../../core";
import { IronSource } from "./IronSource";

export class IronSourceInterstitialAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _displayer: IAsyncHelper<AdResult>;
    private readonly _plugin: IronSource;
    private readonly _network: string;
    private readonly _adId: string;
    private readonly _loader: IAsyncHelper<boolean>;

    public constructor(
        displayer: IAsyncHelper<AdResult>,
        plugin: IronSource,
        network: string;
        adId: string) {
        super();
        this._displayer = displayer;
        this._plugin = plugin;
        this._network = network;
        this._adId = adId;
        this._loader = new AsyncHelper<boolean>();
    }

    public destroy(): void {
        this._plugin.destroyInterstitialAd(this._adId);
    }

    public get isLoaded(): boolean {
        return this._plugin.hasInterstitialAd();
    }

    public load(): Promise<boolean> {
        return this._loader.process(
            () => this._plugin.loadInterstitialAd(),
            result => {
                // OK.
            });
    }

    public show(): Promise<AdResult> {
        return this._displayer.process(
            () => this._plugin.showInterstitialAd(this._adId),
            result => {
                // OK.
            });
    }

    public onLoaded(): void {
        if (this._loader.isProcessing) {
            this._loader.resolve(true);
            this.dispatchEvent(observer => observer.onLoadResult && observer.onLoadResult({
                network: this._network,
                result: true,
                errorCode: 0,
                errorMessage: ``,
            }));
        } else {
            // Assert.
        }
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    public onFailedToLoad(code: number, message: string): void {
        if (this._loader.isProcessing) {
            this._loader.resolve(false);
            this.dispatchEvent(observer => observer.onLoadResult && observer.onLoadResult({
                network: this._network,
                result: true,
                errorCode: code,
                errorMessage: message,
            }));
        } else {
            // Assert.
        }
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

    public onClosed(): void {
        if (this._displayer.isProcessing) {
            this._displayer.resolve(AdResult.Completed);
        } else {
            // Assert.
        }
    }
}