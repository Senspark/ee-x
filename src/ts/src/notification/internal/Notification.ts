import { IMessageBridge, Platform } from "../../core";
import { INotification } from "../INotification";
import { NotificationBuilder } from "../NotificationBuilder";

export class Notification implements INotification {
    private readonly kPrefix = `NotificationBridge`;
    private readonly kSchedule = `${this.kPrefix}Schedule`;
    private readonly kUnschedule = `${this.kPrefix}Unschedule`;
    private readonly kClearAll = `${this.kPrefix}ClearAll`;

    private readonly _bridge: IMessageBridge;

    public constructor(bridge: IMessageBridge) {
        this._bridge = bridge;
    }

    public destroy(): void {
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