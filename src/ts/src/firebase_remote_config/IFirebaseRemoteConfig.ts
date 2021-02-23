import { IPlugin } from "../core";
import { FirebaseFetchStatus } from "./FirebaseFetchStatus";

export interface IFirebaseRemoteConfig extends IPlugin {
    initialize(): Promise<boolean>;
    setSettings(fetchTimeOut: number, fetchInterval: number): Promise<void>;
    fetch(fetchTimeOut: number): Promise<FirebaseFetchStatus>;
    activate(): Promise<boolean>;
    setDefaults(defaults: { [key: string]: boolean | number | string }): Promise<void>;
    getBool(key: string): boolean;
    getLong(key: string): number;
    getDouble(key: string): number;
    getString(key: string): string;
}