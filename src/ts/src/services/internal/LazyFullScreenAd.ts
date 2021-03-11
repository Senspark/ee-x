import {
    AdObserver,
    AdResult,
    IFullScreenAd,
} from "../../ads";
import { ICapper } from "../../ads/internal";
import {
    ObserverHandle,
    ObserverManager,
    Utils,
} from "../../core";

export class LazyFullScreenAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private _ad?: IFullScreenAd;
    private _displayCapper: ICapper;
    private readonly _handle: ObserverHandle;

    public constructor(displayCapper: ICapper) {
        super();
        this._displayCapper = displayCapper;
        this._handle = new ObserverHandle();
    }

    public get ad(): IFullScreenAd | undefined {
        return this._ad;
    }

    public set ad(value: IFullScreenAd | undefined) {
        this._handle.clear();
        if (value == undefined) {
            return;
        }
        this._handle.bind(value)
            .addObserver({
                onLoaded: () => this.dispatchEvent(observer =>
                    observer.onLoaded && observer.onLoaded()),

                onLoadResult: result => this.dispatchEvent(observer =>
                    observer.onLoadResult && observer.onLoadResult(result)),

                onClicked: () => this.dispatchEvent(observer =>
                    observer.onClicked && observer.onClicked()),
            });
        this._ad = value;
        Utils.noAwait(async () => {
            await value.load();
        });
    }

    public destroy(): void {
        this._ad?.destroy();
        this._handle.clear();
    }

    public get isLoaded(): boolean {
        return this._ad?.isLoaded ?? false;
    }

    public async load(): Promise<boolean> {
        if (this._ad == undefined) {
            return false;
        }
        return await this._ad.load();
    }

    public async show(): Promise<AdResult> {
        if (this._ad == undefined) {
            return AdResult.NotInitialized;
        }
        if (this._displayCapper.isCapped) {
            return AdResult.Capped;
        }
        var result = await this._ad.show();
        this._displayCapper.cap();
        return result;
    }
}