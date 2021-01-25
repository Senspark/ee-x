import {
    IMessageBridge,
    Utils,
} from "../../core";
import { MessageHelper } from "./MessageHelper";

export class BannerAdHelper {
    private readonly _bridge: IMessageBridge;
    private readonly _helper: MessageHelper;
    private _anchor: [number, number];
    private _position: [number, number];
    private _size: [number, number];
    private _visible: boolean;

    public constructor(bridge: IMessageBridge, helper: MessageHelper, size: [number, number]) {
        this._bridge = bridge;
        this._helper = helper;
        this._size = size;
        this._anchor = [0, 0];
        this._position = [0, 0];
        this._visible = false;
    }

    public get isLoaded(): boolean {
        const repsonse = this._bridge.call(this._helper.isLoaded);
        return Utils.toBool(repsonse);
    }

    public load(): void {
        this._bridge.call(this._helper.load);
    }

    private setPositionTopLeft(position: [number, number]) {
        const request = {
            [`x`]: Math.floor(position[0]),
            [`y`]: Math.floor(position[1]),
        };
        this._bridge.call(this._helper.setPosition, JSON.stringify(request));
        this._position = position;
    }

    public get anchor(): [number, number] {
        return this._anchor;
    }

    public set anchor(value: [number, number]) {
        this.setPositionTopLeft([
            this._position[0] - (value[0] - this._anchor[0]) * this._size[0],
            this._position[1] - (value[1] - this._anchor[1]) * this._size[1]]);
        this._anchor = value;
    }

    public get position(): [number, number] {
        return [
            this._position[0] + this._anchor[0] * this._size[0],
            this._position[1] + this._anchor[1] * this._size[1]];
    }

    public set position(value: [number, number]) {
        this.setPositionTopLeft([
            value[0] - this._anchor[0] * this._size[0],
            value[1] - this._anchor[1] * this._size[1]]);
    }

    public get size(): [number, number] {
        return this._size;
    }

    public set size(value: [number, number]) {
        this.setPositionTopLeft([
            this._position[0] - (value[0] - this._size[0]) * this._anchor[0],
            this._position[1] - (value[1] - this._size[1]) * this._anchor[1]]);
        const request = {
            [`width`]: Math.floor(value[0]),
            [`height`]: Math.floor(value[1]),
        };
        this._bridge.call(this._helper.setSize, JSON.stringify(request));
        this._size = value;
    }

    public get isVisible(): boolean {
        return this._visible;
    }

    public set isVisible(value: boolean) {
        this._bridge.call(this._helper.setVisible, Utils.toString(value));
        this._visible = value;
    }
}