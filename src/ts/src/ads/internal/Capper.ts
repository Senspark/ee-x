import { Utils } from "../../core";
import { ICapper } from "./ICapper";

export class Capper implements ICapper {
    private _interval: number;
    private _capped: boolean;

    public constructor(interval: number) {
        this._interval = interval;
        this._capped = false;
    }

    public get isCapped(): boolean {
        return this._capped;
    }

    public cap(): void {
        this._capped = true;
        Utils.noAwait(async () => {
            await Utils.delay(this._interval);
            this._capped = false;
        });
    }
}