import {
    ILogger,
    IMessageBridge,
} from "../../core";
import { IFirebaseAnalytics } from "../IFirebaseAnalytics";

type Destroyer = () => void;

export class FirebaseAnalytics implements IFirebaseAnalytics {
    private readonly kTag = `FirebaseAnalytics`;
    private readonly kPrefix = "FirebaseAnalyticsBridge";
    private readonly kSetUserProperty = `${this.kPrefix}SetUserProperty`;
    private readonly kTrackScreen = `${this.kPrefix}TrackScreen`;
    private readonly kLogEvent = `${this.kPrefix}LogEvent`;

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

    public setUserProperty(key: string, value: string): void {
        const request = {
            key,
            value,
        };
        this._bridge.call(this.kSetUserProperty, JSON.stringify(request));
    }

    public trackScreen(name: string): void {
        this._bridge.call(this.kTrackScreen, name);
    }

    public logEvent(name: string, parameters: { [key: string]: string | number; }): void {
        const request = {
            name,
            parameters,
        };
        this._bridge.call(this.kLogEvent, JSON.stringify(request));
    }
}