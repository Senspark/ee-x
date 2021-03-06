import { IPlugin } from "../core";

export interface IFirebaseAnalytics extends IPlugin {
    initialize(): Promise<boolean>;
    setUserProperty(key: string, value: string): void;
    trackScreen(name: string): void;
    logEvent(name: string, parameters: { [key: string]: number | string }): void;
}