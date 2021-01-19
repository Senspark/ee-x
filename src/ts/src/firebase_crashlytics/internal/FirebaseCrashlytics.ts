import { IMessageBridge } from "../../core";
import { IFirebaseCrashlytics } from "../IFirebaseCrashlytics";

export class FirebaseCrashlytics implements IFirebaseCrashlytics {
    private readonly kPrefix = "FirebaseCrashlyticsBridge";
    private readonly kLog = this.kPrefix + "Log";

    private readonly _bridge: IMessageBridge;

    public constructor(bridge: IMessageBridge) {
        this._bridge = bridge;
    }

    public destroy(): void {
    }

    public log(message: string): void {
        this._bridge.call(this.kLog, message);
    }
}