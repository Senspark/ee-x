import { FullScreenAdResult } from "../IFullScreenAd";
import { AsyncHelper } from "./AsyncHelper";
import { IAsyncHelper } from "./IAsyncHelper";

export class MediationManager {
    private static _sharedInstance = new MediationManager();

    private _adDisplayer: IAsyncHelper<FullScreenAdResult>;

    public static getInstance(): MediationManager {
        return this._sharedInstance;
    }

    private constructor() {
        this._adDisplayer = new AsyncHelper<FullScreenAdResult>();
    }

    public get adDisplayer(): IAsyncHelper<FullScreenAdResult> {
        return this._adDisplayer;
    }
}