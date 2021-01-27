import {
    ILogger,
    IMessageBridge,
} from "../../core";
import { IFirebaseCrashlytics } from "../IFirebaseCrashlytics";

type Destroyer = () => void;

export class FirebaseCrashlytics implements IFirebaseCrashlytics {
    private readonly kTag = `FirebaseCrashlytics`;
    private readonly kPrefix = "FirebaseCrashlyticsBridge";
    private readonly kLog = this.kPrefix + "Log";

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

    public log(message: string): void {
        this._bridge.call(this.kLog, message);
    }
}