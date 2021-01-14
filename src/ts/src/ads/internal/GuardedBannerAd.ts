import { ObserverHandle } from "../../core/ObserverHandle";
import { ObserverManager } from "../../core/ObserverManager";
import { AdObserver } from "../IAd";
import { IBannerAd } from "../IBannerAd";

export class GuardedBannerAd extends ObserverManager<AdObserver> implements IBannerAd {
    private readonly _ad: IBannerAd;
    private _loaded: boolean;
    private _loading: boolean;
    private readonly _handle: ObserverHandle;

    public constructor(ad: IBannerAd) {
        super();
        this._ad = ad;
        this._loaded = false;
        this._loading = false;
        this._handle = new ObserverHandle();
        this._handle.bind(ad).addObserver({
            onLoaded: () => this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded()),
            onClicked: () => this.dispatchEvent(observer => observer.onClicked && observer.onClicked()),
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
        if (this._loading) {
            return await this._ad.load();
        }
        this._loading = true;
        this._loaded = await this._ad.load();
        this._loading = false;
        return this._loaded;
    }

    public get anchor(): [number, number] {
        return this._ad.anchor;
    }

    public set anchor(value: [number, number]) {
        this._ad.anchor = value;
    }

    public get position(): [number, number] {
        return this._ad.position;
    }

    public set position(value: [number, number]) {
        this._ad.position = value;
    }

    public get size(): [number, number] {
        return this._ad.size;
    }

    public set size(value: [number, number]) {
        this._ad.size = value;
    }

    public get isVisible(): boolean {
        return this._ad.isVisible;
    }

    public set isVisible(value: boolean) {
        this._ad.isVisible = value;
    }
}