import {
    AdObserver,
    FullScreenAdResult,
    IAd,
    IFullScreenAd,
} from "../../ads";
import {
    ObserverHandle,
    ObserverManager,
    Platform,
    Utils,
} from "../../core";
import { AdResult } from "../AdResult";

export class GenericAd extends ObserverManager<AdObserver> implements IAd {
    private readonly _ad: IFullScreenAd;
    private _capped: boolean;
    private readonly _interval: number;
    private readonly _handle: ObserverHandle;

    public constructor(ad: IFullScreenAd, interval: number) {
        super();
        this._ad = ad;
        this._interval = interval;
        this._capped = false;
        Utils.noAwait(async () => {
            await this._ad.load();
        });
        this.updateCapping();
        this._handle = new ObserverHandle();
        this._handle.bind(this._ad)
            .addObserver({
                onLoaded: () => this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded()),
                onClicked: () => this.dispatchEvent(observer => observer.onClicked && observer.onClicked()),
            });
    }

    public destroy(): void {
        this._ad.destroy();
        this._handle.clear();
    }

    public get isLoaded(): boolean {
        return this._ad.isLoaded;
    };

    public async load(): Promise<boolean> {
        return await this._ad.load();
    }

    public async show(): Promise<AdResult> {
        if (this._interval > 0 && this._capped) {
            return AdResult.Capped;
        }
        if (this._ad.isLoaded) {
            const hasInternet = await this.testConnection(0.2);
            if (hasInternet) {
                // OK.
            } else {
                return AdResult.NoInternet;
            }
        } else {
            Utils.noAwait(async () => { //
                await this._ad.load()
            });
            let hasInternet = false;
            await Promise.all([
                Utils.delay(0.1),
                (async () => { //
                    hasInternet = await this.testConnection(0.2);
                })(),
            ]);
            if (hasInternet) {
                // OK.
            } else {
                return AdResult.NoInternet;
            }
        }
        if (this._ad.isLoaded) {
            // OK.
        } else {
            return AdResult.NotLoaded;
        }
        const result = await this._ad.show();
        Utils.noAwait(async () => {
            await this._ad.load();
        });
        switch (result) {
            case FullScreenAdResult.Completed:
                this.updateCapping();
                return AdResult.Completed;
            case FullScreenAdResult.Canceled:
                return AdResult.Canceled;
            case FullScreenAdResult.Failed:
                return AdResult.Failed;
        }
        // Assert.
        return AdResult.Failed;
    }

    private updateCapping(): void {
        if (this._interval > 0) {
            this._capped = true;
            Utils.noAwait(async () => {
                await Utils.delay(this._interval);
                this._capped = false;
            });
        }
    }

    private async testConnection(timeOut: number): Promise<boolean> {
        return await Platform.testConnection("www.google.com", timeOut);
    }
}