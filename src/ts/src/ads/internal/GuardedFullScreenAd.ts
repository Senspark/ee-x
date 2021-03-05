import {
    ObserverHandle,
    ObserverManager,
} from "../../core";
import { AdObserver } from "../IAd";
import {
    AdResult,
    IFullScreenAd,
} from "../IFullScreenAd";

export class GuardedFullScreenAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _ad: IFullScreenAd;
    private _loaded: boolean;
    private _loading: boolean;
    private _displaying: boolean;
    private readonly _handle: ObserverHandle;

    public constructor(ad: IFullScreenAd) {
        super();
        this._ad = ad;
        this._loaded = false;
        this._loading = false;
        this._displaying = false;
        this._handle = new ObserverHandle();
        this._handle.bind(ad).addObserver({
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
        return this._loaded;
    }

    public async load(): Promise<boolean> {
        if (this._loaded) {
            return true;
        }
        if (this._displaying) {
            return false;
        }
        if (this._loading) {
            return false;
        }
        this._loading = true;
        this._loaded = await this._ad.load();
        this._loading = false;
        return this._loaded;
    }

    public async show(): Promise<AdResult> {
        if (!this._loaded) {
            return AdResult.Failed;
        }
        if (this._loading) {
            return AdResult.Failed;
        }
        if (this._displaying) {
            return AdResult.Failed;
        }
        this._displaying = true;
        this._loaded = false;
        const result = await this._ad.show();
        this._displaying = false;
        return result;
    }
}