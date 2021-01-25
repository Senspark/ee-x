import { NotificationBuilder } from "./NotificationBuilder";

export interface INotification {
    schedule(builder: NotificationBuilder): void;
    unschedule(tag: number): void;
    clearAll(): void;
}