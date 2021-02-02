import { IPlugin } from "../core";
import { FirebaseFetchStatus } from "./FirebaseFetchStatus";

export interface IFirebaseRemoteConfig extends IPlugin {
    initialize(): Promise<void>;
    setSettings(fetchTimeOut: number, fetchInterval: number): Promise<void>;
    fetch(fetchTimeOut: number): Promise<FirebaseFetchStatus>;
    activate(): Promise<boolean>;
    setDefaults(defaults: { [key: string]: boolean | number | string }): Promise<void>;
    getBool(key: string): Promise<boolean>;
    getLong(key: string): Promise<number>;
    getDouble(key: string): Promise<number>;
    getString(key: string): Promise<string>;
}