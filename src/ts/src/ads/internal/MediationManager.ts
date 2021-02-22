import { AdResult } from "../IFullScreenAd";
import { AsyncHelper } from "./AsyncHelper";
import { IAsyncHelper } from "./IAsyncHelper";

export class MediationManager {
    private static _sharedInstance = new MediationManager();

    private _adDisplayer: IAsyncHelper<AdResult>;

    public static getInstance(): MediationManager {
        return this._sharedInstance;
    }

    private constructor() {
        this._adDisplayer = new AsyncHelper<AdResult>();
    }

    public get adDisplayer(): IAsyncHelper<AdResult> {
        return this._adDisplayer;
    }
}