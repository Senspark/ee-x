import {IPlugin, ILibraryAnalytics} from "../core";

export interface IAppsFlyer extends IPlugin, ILibraryAnalytics {
    initialize(devKey: string, iosAppId: string, appIdentity: string): void;

    startTracking(): void;

    getDeviceId(): string;

    setDebugEnabled(enabled: boolean): void;

    setStopTracking(enabled: boolean): void;

    trackEvent(name: string, parameters: { [key: string]: number | string }): void;
}