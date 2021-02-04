import { IPlugin } from "../core";

export interface IFirebaseCrashlytics extends IPlugin {
    initialize(): Promise<boolean>;
    log(message: string): void;
}