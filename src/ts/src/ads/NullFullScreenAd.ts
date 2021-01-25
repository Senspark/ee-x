import { ObserverManager } from "../core";
import { AdObserver } from "./IAd";
import {
    FullScreenAdResult,
    IFullScreenAd,
} from "./IFullScreenAd";

export class NullFullScreenAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    public destroy(): void {
    }

    public get isLoaded(): boolean {
        return false;
    }

    public async load(): Promise<boolean> {
        return false;
    }

    public async show(): Promise<FullScreenAdResult> {
        return FullScreenAdResult.Failed;
    }
}