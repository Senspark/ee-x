import {
    ObserverHandle,
    ObserverManager,
} from "../core";
import { AdObserver } from "./IAd";
import { IBannerAd } from "./IBannerAd";
import { IMultiAd } from "./IMultiAd";

export class MultiBannerAd extends ObserverManager<AdObserver> implements IBannerAd, IMultiAd<IBannerAd> {
    private _visible: boolean;
    private _anchor: [number, number];
    private _position: [number, number];
    private readonly _items: IBannerAd[];
    private readonly _handle: ObserverHandle;
    private _activeItem?: IBannerAd;
    private _loadedItems: IBannerAd[];

    public constructor() {
        super();
        this._anchor = [0, 0];
        this._position = [0, 0];
        this._visible = false;
        this._items = [];
        this._handle = new ObserverHandle();
        this._loadedItems = [];
    }

    public addItem(item: IBannerAd): void {
        this._items.push(item);
        item.isVisible = this._visible;
        this._handle.bind(item).addObserver({
            onLoaded: () => {
                if (this._loadedItems.indexOf(item) === -1) {
                    this._loadedItems.push(item);
                }
                this.invalidate();
                this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
            },
            onClicked: () => this.dispatchEvent(observer => observer.onClicked && observer.onClicked()),
        });
    }

    public destroy(): void {
        for (const item of this._items) {
            item.destroy();
        }
        this._items.length = 0;
        this._handle.clear();
        this._activeItem = undefined;
    }

    public get isLoaded(): boolean {
        return this._items.some(item => item.isLoaded);
    }

    public async load(): Promise<boolean> {
        let result = false;
        for (const item of this._items) {
            if (item === this._activeItem && this._visible) {
                continue;
            }
            if (this._loadedItems.indexOf(item) !== -1) {
                continue;
            }
            if (await item.load()) {
                result = true;
            }
        }
        return true;
    }

    public get isVisible(): boolean {
        return this._visible;
    }

    public set isVisible(value: boolean) {
        this._visible = value;
        this.invalidate();
        for (const item of this._items) {
            if (this._loadedItems.indexOf(item) === -1) {
                // Load in background.ss
                const _ = item.load();
            }
        }
    }

    public get anchor(): [number, number] {
        return this._anchor;
    }

    public set anchor(value: [number, number]) {
        this._anchor = value;
        for (const item of this._items) {
            item.anchor = value;
        }
    }

    public get position(): [number, number] {
        return this._position;
    }

    public set position(value: [number, number]) {
        this._position = value;
        for (const item of this._items) {
            item.position = value;
        }
    }

    public get size(): [number, number] {
        let width = 0;
        let height = 0;
        for (const item of this._items) {
            const [itemWidth, itemHeight] = item.size;
            width = Math.max(width, itemWidth);
            height = Math.max(height, itemHeight);
        }
        return [width, height];
    }

    public set size(value: [number, number]) {
        for (const item of this._items) {
            item.size = value;
        }
    }

    private invalidate(): void {
        const lastActiveItem = this._activeItem;
        for (const item of this._items) {
            if (this._loadedItems.indexOf(item) !== -1) {
                this._activeItem = item;
                break;
            }
        }
        if (this._activeItem !== undefined) {
            this._activeItem.isVisible = this._visible;
        }
        if (lastActiveItem !== undefined && lastActiveItem !== this._activeItem) {
            lastActiveItem.isVisible = false;
        }
    }
}