import {
    AdObserver,
    AdResult,
    IAd,
    IFullScreenAd,
} from "../../ads";
import {
    Capper,
    ICapper,
} from "../../ads/internal";
import {
    ObserverHandle,
    ObserverManager,
    Platform,
    Utils,
} from "../../core";

export class GenericAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _ad: IFullScreenAd;
    private readonly _capper: ICapper;
    private readonly _handle: ObserverHandle;

    public constructor(ad: IFullScreenAd, interval: number) {
        super();
        this._ad = ad;
        this._capper = new Capper(interval);
        this._handle = new ObserverHandle();
        this._handle.bind(this._ad)
            .addObserver({
                onLoaded: () => this.dispatchEvent(observer =>
                    observer.onLoaded && observer.onLoaded()),

                onLoadResult: result => this.dispatchEvent(observer =>
                    observer.onLoadResult && observer.onLoadResult(result)),

                onClicked: () => this.dispatchEvent(observer =>
                    observer.onClicked && observer.onClicked()),
            });
    }

    public destroy(): void {
        this._ad.destroy();
        this._handle.clear();
    }

    public get isLoaded(): boolean {
        return this._ad.isLoaded;
    };

    public async load(): Promise<boolean> {
        return await this._ad.load();
    }

    public async show(): Promise<AdResult> {
        if (this._capper.isCapped) {
            return AdResult.Capped;
        }
        if (this._ad.isLoaded) {
            const hasInternet = await this.testConnection(0.2);
            if (hasInternet) {
                // OK.
            } else {
                return AdResult.NoInternet;
            }
        } else {
            Utils.noAwait(async () => { //
                await this._ad.load()
            });
            let hasInternet = false;
            await Promise.all([
                Utils.delay(0.1),
                (async () => { //
                    hasInternet = await this.testConnection(0.2);
                })(),
            ]);
            if (hasInternet) {
                // OK.
            } else {
                return AdResult.NoInternet;
            }
        }
        if (this._ad.isLoaded) {
            // OK.
        } else {
            return AdResult.NotLoaded;
        }
        const result = await this._ad.show();
        Utils.noAwait(async () => {
            await this._ad.load();
        });
        if (result == AdResult.Completed) {
            this._capper.cap();
        }
        return result;
    }

    private async testConnection(timeOut: number): Promise<boolean> {
        return await Platform.testConnection("www.google.com", timeOut);
    }
}