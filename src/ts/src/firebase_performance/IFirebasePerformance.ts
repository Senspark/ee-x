import { IPlugin } from "../core";
import { IFirebasePerformanceTrace } from "./IFirebasePerformanceTrace";

export interface IFirebasePerformance extends IPlugin {
    isDataCollectionEnabled: boolean;
    newTrace(name: string): IFirebasePerformanceTrace;
}