import { IObserverManager } from "../core";

export interface AdLoadResult {
    network: string,
    result: boolean,
    errorCode: number,
    errorMessage: string,
}

export interface AdObserver {
    onLoaded?(): void;
    onLoadResult?(result: AdLoadResult): void
    onClicked?(): void;
}

export interface IAd extends IObserverManager<AdObserver> {
    destroy(): void;
    isLoaded: boolean;
    load(): Promise<boolean>;
}