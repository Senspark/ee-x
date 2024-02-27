import { IPlugin, ILibraryAnalytics } from "../core";

export interface IFirebaseAnalytics extends IPlugin, ILibraryAnalytics {
    initialize(appIdentity: string): Promise<boolean>;
    setUserProperty(key: string, value: string): void;
    trackScreen(name: string): void;
    logEvent(name: string, parameters: { [key: string]: number | string }): void;
}