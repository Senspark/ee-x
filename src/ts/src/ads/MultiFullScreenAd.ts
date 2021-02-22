import {
    ObserverHandle,
    ObserverManager,
} from "../core";
import { AdObserver } from "./IAd";
import {
    AdResult,
    IFullScreenAd,
} from "./IFullScreenAd";
import { IMultiAd } from "./IMultiAd";

export class MultiFullScreenAd extends ObserverManager<AdObserver> implements IFullScreenAd, IMultiAd<IFullScreenAd> {
    private readonly _items: IFullScreenAd[];
    private readonly _handle: ObserverHandle;

    public constructor() {
        super();
        this._items = [];
        this._handle = new ObserverHandle();
    }

    public addItem(item: IFullScreenAd): void {
        this._items.push(item);
        this._handle.bind(item).addObserver({
            onLoaded: () => this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded()),
            onClicked: () => this.dispatchEvent(observer => observer.onClicked && observer.onClicked()),
        });
    }

    public destroy(): void {
        for (const item of this._items) {
            item.destroy();
        }
        this._items.length = 0;
        this._handle.clear();
    }

    public get isLoaded(): boolean {
        return this._items.some(item => item.isLoaded);
    }

    public async load(): Promise<boolean> {
        let result = false;
        for (const item of this._items) {
            if (item.isLoaded) {
                continue;
            }
            if (await item.load()) {
                result = true;
            }
        }
        return result;
    }

    public async show(): Promise<AdResult> {
        let result = AdResult.Failed;
        for (const item of this._items) {
            if (result === AdResult.Failed) {
                result = await item.show();
            }
            if (!item.isLoaded) {
                // Load in background.
                const _ = item.load();
            }
        }
        return result;
    }
}