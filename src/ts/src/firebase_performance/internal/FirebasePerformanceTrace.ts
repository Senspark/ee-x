import { IMessageBridge } from "../../core";
import { IFirebasePerformanceTrace } from "../IFirebasePerformanceTrace";
import { FirebasePerformance } from "./FirebasePerformance";

export class FirebasePerformanceTrace implements IFirebasePerformanceTrace {
    private readonly _bridge: IMessageBridge;
    private readonly _plugin: FirebasePerformance;
    private readonly _name: string;

    public constructor(bridge: IMessageBridge, plugin: FirebasePerformance, name: string) {
        this._bridge = bridge;
        this._plugin = plugin;
        this._name = name;
    }

    public start(): void {
        this._bridge.call(`FirebasePerformance_start_${this._name}`);
    }

    public stop(): void {
        this._bridge.call(`FirebasePerformance_stop_${this._name}`);
    }

    public putMetric(metricName: string, value: number): void {
        const request = {
            key: metricName,
            value: value,
        };
        this._bridge.call(`FirebasePerformance_putMetric_${this._name}`, JSON.stringify(request));
    }

    public incrementMetric(metricName: string, incrementBy: number): void {
        const request = {
            key: metricName,
            value: incrementBy,
        };
        this._bridge.call(`FirebasePerformance_incrementMetric_${this._name}`, JSON.stringify(request));
    }

    public getLongMetric(metricName: string): number {
        const response = this._bridge.call(`FirebasePerformance_getLongMetric_${this._name}`, metricName);
        return parseInt(response);
    }
}