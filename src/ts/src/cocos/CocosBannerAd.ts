import {
    AdObserver,
    IBannerAd,
} from "../ads";
import {
    ObserverHandle,
    ObserverManager,
    Platform,
} from "../core";

export class CocosBannerAd extends ObserverManager<AdObserver> implements IBannerAd {
    private readonly _ad: IBannerAd;
    private readonly _handle: ObserverHandle;
    private readonly _screenHeight: number;

    public constructor(ad: IBannerAd) {
        super();
        this._ad = ad;
        this._handle = new ObserverHandle();
        this._handle.bind(ad).addObserver({
            onLoaded: () => this.dispatchEvent(observer =>
                observer.onLoaded && observer.onLoaded()),

            onLoadResult: result => this.dispatchEvent(observer =>
                observer.onLoadResult && observer.onLoadResult(result)),

            onClicked: () => this.dispatchEvent(observer =>
                observer.onClicked && observer.onClicked()),
        });
        this._screenHeight = Platform.getViewSize()[1];
    }

    public destroy(): void {
        this._ad.destroy();
        this._handle.clear();
    }

    public get isLoaded(): boolean {
        return this._ad.isLoaded;
    }

    public async load(): Promise<boolean> {
        return await this._ad.load();
    }

    public get anchor(): [number, number] {
        const [x, y] = this._ad.anchor;
        return [x, 1 - y];
    }

    public set anchor(value: [number, number]) {
        const [x, y] = value;
        this._ad.anchor = [x, 1 - y];
    }

    public get position(): [number, number] {
        const [x, y] = this._ad.position;
        return [x, this._screenHeight - y];
    }

    public set position(value: [number, number]) {
        const [x, y] = value;
        this._ad.position = [x, this._screenHeight - y];
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