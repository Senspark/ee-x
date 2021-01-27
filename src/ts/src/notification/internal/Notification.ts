import {
    ILogger,
    IMessageBridge,
    Platform,
} from "../../core";
import { INotification } from "../INotification";
import { NotificationBuilder } from "../NotificationBuilder";

type Destroyer = () => void;

export class Notification implements INotification {
    private readonly kPrefix = `NotificationBridge`;
    private readonly kSchedule = `${this.kPrefix}Schedule`;
    private readonly kUnschedule = `${this.kPrefix}Unschedule`;
    private readonly kClearAll = `${this.kPrefix}ClearAll`;

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

    public schedule(builder: NotificationBuilder): void {
        const request = {
            ticker: builder.ticker,
            title: builder.title.length === 0 ? Platform.getApplicationName() : builder.title,
            body: builder.body,
            delay: builder.delay,
            interval: builder.interval,
            tag: builder.tag,
        };
        this._bridge.call(this.kSchedule, JSON.stringify(request));
    }

    public unschedule(tag: number): void {
        const request = {
            tag: tag,
        };
        this._bridge.call(this.kUnschedule, JSON.stringify(request));
    }

    public clearAll(): void {
        this._bridge.call(this.kClearAll);
    }
}