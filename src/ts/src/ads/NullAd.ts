import { ObserverManager } from "../core";
import { AdObserver, IAd } from "./IAd";

export class NullAd extends ObserverManager<AdObserver> implements IAd {
    public destroy(): void {
    }

    public get isLoaded(): boolean {
        return false;
    }

    public async load(): Promise<boolean> {
        return false;
    }
}