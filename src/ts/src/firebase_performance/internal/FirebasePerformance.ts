import {
    ILogger,
    IMessageBridge,
    Utils,
} from "../../core";
import { IFirebasePerformance } from "../IFirebasePerformance";
import { IFirebasePerformanceTrace } from "../IFirebasePerformanceTrace";
import { FirebasePerformanceTrace } from "./FirebasePerformanceTrace";

type Destroyer = () => void;

export class FirebasePerformance implements IFirebasePerformance {
    private readonly kTag = `FirebasePerformance`;
    private readonly kPrefix = "FirebasePerformanceBridge";
    private readonly kSetDataCollectionEnabled = `${this.kPrefix}SetDataCollectionEnabled`;
    private readonly kIsDataCollectionEnabled = `${this.kPrefix}IsDataCollectionEnabled`;
    private readonly kNewTrace = `${this.kPrefix}NewTrace`;

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
        this._logger.debug(`${this.kTag}: constructor`);
    }

    public destroy(): void {
        this._logger.debug(`${this.kTag}: destroy`);
        this._destroyer();
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