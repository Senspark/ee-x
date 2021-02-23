import { IPlugin } from "../core";
import { NotificationBuilder } from "./NotificationBuilder";

export interface INotification extends IPlugin {
    schedule(builder: NotificationBuilder): void;
    unschedule(tag: number): void;
    clearAll(): void;
}