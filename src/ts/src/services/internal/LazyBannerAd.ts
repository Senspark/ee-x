import {
    AdObserver,
    IBannerAd,
} from "../../ads";
import {
    ObserverHandle,
    ObserverManager,
    Utils,
} from "../../core";

export class LazyBannerAd extends ObserverManager<AdObserver> implements IBannerAd {
    private _ad?: IBannerAd;
    private _visible: boolean;
    private _anchor: [number, number];
    private _position: [number, number];
    private _size?: [number, number];
    private readonly _handle: ObserverHandle;

    public constructor() {
        super();
        this._visible = false;
        this._anchor = [0, 0];
        this._position = [0, 0];
        this._size = undefined;
        this._handle = new ObserverHandle();
    }

    public get ad(): IBannerAd | undefined {
        return this._ad;
    }

    public set ad(value: IBannerAd | undefined) {
        this._handle.clear();
        if (value === undefined) {
            return;
        }
        this._handle.bind(value)
            .addObserver({
                onLoaded: () => {
                    if (this._visible) {
                        value.isVisible = false;
                        value.isVisible = true;
                    }
                    this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
                },

                onLoadResult: result => this.dispatchEvent(observer =>
                    observer.onLoadResult && observer.onLoadResult(result)),

                onClicked: () => this.dispatchEvent(observer =>
                    observer.onClicked && observer.onClicked()),
            });
        this._ad = value;
        this._ad.isVisible = this._visible;
        this._ad.anchor = this._anchor;
        this._ad.position = this._position;
        if (this._size !== undefined) {
            this._ad.size = this._size;
        }
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

    public get anchor(): [number, number] {
        return this._anchor;
    }

    public set anchor(value: [number, number]) {
        this._anchor = value;
        if (this._ad !== undefined) {
            this._ad.anchor = value;
        }
    }

    public get position(): [number, number] {
        return this._position;
    }

    public set position(value: [number, number]) {
        this._position = value;
        if (this._ad !== undefined) {
            this._ad.position = value;
        }
    }

    public get size(): [number, number] {
        if (this._size !== undefined) {
            return this._size;
        }
        return this._ad?.size ?? [0, 0];
    }

    public set size(value: [number, number]) {
        this._size = value;
        if (this._ad !== undefined) {
            this._ad.size = value;
        }
    }

    public get isVisible(): boolean {
        return this._visible;
    }

    public set isVisible(value: boolean) {
        this._visible = value;
        if (this._ad !== undefined) {
            this._ad.isVisible = value;
        }
    }
}