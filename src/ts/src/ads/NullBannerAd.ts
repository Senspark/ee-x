import { IBannerAd } from "./IBannerAd";
import { NullAd } from "./NullAd";

export class NullBannerAd extends NullAd implements IBannerAd {
    private _visible: boolean;
    private _anchor: [number, number];
    private _position: [number, number];
    private _size: [number, number];

    public constructor() {
        super();
        this._visible = false;
        this._anchor = [0, 0];
        this._position = [0, 0];
        this._size = [0, 0];
    }

    public get isVisible(): boolean {
        return this._visible;
    }

    public set isVisible(value: boolean) {
        this._visible = value;
    }

    public get anchor(): [number, number] {
        return this._anchor;
    }

    public set anchor(value: [number, number]) {
        this._position[0] -= (value[0] - this._anchor[0]) * this._size[0];
        this._position[1] -= (value[1] - this._anchor[1]) * this._size[1];
        this._anchor = value;
    }

    public get position(): [number, number] {
        return [
            this._position[0] + this._anchor[0] * this._size[0],
            this._position[1] + this._anchor[1] * this._size[1]];
    }

    public set position(value: [number, number]) {
        this._position = [
            value[0] - this._anchor[0] * this._size[0],
            value[1] - this._anchor[1] * this._size[1]];
    }

    public get size(): [number, number] {
        return this._size;
    }

    public set size(value: [number, number]) {
        this._position[0] -= (value[0] - this._size[0]) * this._anchor[0];
        this._position[1] -= (value[1] - this._size[1]) * this._anchor[1];
        this._size = value;
    }
}