import { IMessageBridge, Utils } from "../../core";
import { IFirebasePerformance } from "../IFirebasePerformance";
import { IFirebasePerformanceTrace } from "../IFirebasePerformanceTrace";
import { FirebasePerformanceTrace } from "./FirebasePerformanceTrace";

export class FirebasePerformance implements IFirebasePerformance {
    private readonly kPrefix = "FirebasePerformanceBridge";
    private readonly kSetDataCollectionEnabled = `${this.kPrefix}SetDataCollectionEnabled`;
    private readonly kIsDataCollectionEnabled = `${this.kPrefix}IsDataCollectionEnabled`;
    private readonly kNewTrace = `${this.kPrefix}NewTrace`;

    private readonly _bridge: IMessageBridge;

    public constructor(bridge: IMessageBridge) {
        this._bridge = bridge;
    }

    public destroy(): void {
    }

    public get isDataCollectionEnabled(): boolean {
        const response = this._bridge.call(this.kIsDataCollectionEnabled);
        return Utils.toBool(response);
    }

    public set isDataCollectionEnabled(value: boolean) {
        this._bridge.call(this.kSetDataCollectionEnabled, Utils.toString(value));
    }

    public newTrace(name: string): IFirebasePerformanceTrace {
        const response = this._bridge.call(this.kNewTrace, name);
        if (!Utils.toBool(response)) {
            throw new Error(`Cannot create a new trace`);
        }
        return new FirebasePerformanceTrace(this._bridge, this, name);
    }
}