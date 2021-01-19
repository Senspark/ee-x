import { IPlugin } from "../core";

export interface IFirebaseCrashlytics extends IPlugin {
    log(message: string): void;
}