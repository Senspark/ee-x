import { IPlugin } from "../core";
import { IFirebasePerformanceTrace } from "./IFirebasePerformanceTrace";

export interface IFirebasePerformance extends IPlugin {
    initialize(): Promise<boolean>;
    isDataCollectionEnabled: boolean;
    newTrace(name: string): IFirebasePerformanceTrace;
}