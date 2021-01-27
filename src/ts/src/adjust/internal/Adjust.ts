import {
    ILogger,
    IMessageBridge,
    Utils,
} from "../../core";
import { AdjustConfig } from "../AdjustConfig";
import { IAdjust } from "../IAdjust";

type Destroyer = () => void;

export class Adjust implements IAdjust {
    private readonly kPrefix = "AdjustBridge";
    private readonly kInitialize = this.kPrefix + "Initialize";
    private readonly kSetEnabled = this.kPrefix + "SetEnabled";
    private readonly kGetAdvertisingIdentifier = this.kPrefix + "GetAdvertisingIdentifier";
    private readonly kGetDeviceIdentifier = this.kPrefix + "GetDeviceIdentifier";
    private readonly kSetPushToken = this.kPrefix + "SetPushToken";
    private readonly kTrackEvent = this.kPrefix + "TrackEvent";

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
    }

    public destroy(): void {
        this._destroyer();
    }

    public initialize(config: AdjustConfig): void {
        const request: {
            token: string,
            environment: number,
            logLevel: number,
            eventBufferingEnabled: boolean
        } = {
            token: config.token,
            environment: config.environment,
            logLevel: config.logLevel,
            eventBufferingEnabled: config.eventBufferingEnabled,
        };
        this._bridge.call(this.kInitialize, JSON.stringify(request));
    }

    public setEnabled(enabled: boolean): void {
        this._bridge.call(this.kSetEnabled, Utils.toString(enabled));
    }

    public async getAdvertisingIdentifier(): Promise<string> {
        const response = await this._bridge.callAsync(this.kGetAdvertisingIdentifier);
        return response;
    }

    public getDeviceIdentifier(): string {
        const response = this._bridge.call(this.kGetDeviceIdentifier);
        return response;
    }

    public setPushToken(token: string): void {
        this._bridge.call(this.kSetPushToken, token);
    }

    public trackEvent(token: string): void {
        this._bridge.call(this.kTrackEvent, token);
    }
}