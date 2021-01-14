import { IObserverManager } from "../core/IObserverManager";

export interface AdObserver {
    onLoaded?(): void;
    onClicked?(): void;
}

export interface IAd extends IObserverManager<AdObserver> {
    destroy(): void;
    isLoaded: boolean;
    load(): Promise<boolean>;
}