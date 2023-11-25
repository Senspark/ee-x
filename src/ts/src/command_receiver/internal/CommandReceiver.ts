import {ILogger, IMessageBridge, ObserverManager, Platform,} from "../../core";
import {ICommandObserver, ICommandReceiver} from "../ICommandReceiver";

type Destroyer = () => void;

export interface ICmdData {
    cmd: string;
    desc: string;
    sample: string;
}

export class CommandReceiver extends ObserverManager<ICommandObserver> implements ICommandReceiver {
    private readonly kTag = `MReceiver`;
    private readonly kPrefix = `${this.kTag}Bridge`;
    private readonly kOnMessageReceived = `${this.kPrefix}OnMessageReceived`;
    private readonly kAddCommand = `${this.kPrefix}AddCommand`;
    private readonly kShowUI = `${this.kPrefix}ShowUI`;

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;
    private readonly _pendingCmd: ICmdData[] = [];

    private _isValidated = false;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        super();
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
        this._logger.debug(`${this.kTag}: constructor`);
    }

    public destroy(): void {
        this._logger.debug(`${this.kTag}: destroy`);
        this._destroyer();
    }

    public initialize(): void {
        this._bridge.registerHandler((json: string) => {
            this.onMessageReceived(json);
        }, this.kOnMessageReceived);

        // validating if device comes from senspark company ?
        const appCheckName = "com.senspark.dev.tools";
        const isInstalled = Platform.isApplicationInstalled(appCheckName);
        if (isInstalled) {
            Platform.openApplication(appCheckName);
            this._bridge.call(this.kShowUI);
            this._isValidated = true;

            while (this._pendingCmd.length > 0) {
                const cmd = this._pendingCmd.pop();
                if (cmd) {
                    this.addCommand(cmd);
                }
            }
        }
    }

    public addCommand(data: ICmdData): void {
        if (this._isValidated) {
            this._bridge.call(this.kAddCommand, JSON.stringify(data));
        } else {
            this._pendingCmd.push(data);
        }
    }

    private onMessageReceived(json: string): void {
        this._logger.debug(`${this.kTag}: onMessageReceived: ${json}`);
        try {
            const message = JSON.parse(json);
            const cmd = message["cmd"];
            if (!cmd || cmd === "") {
                return;
            }
            let data = message["data"];
            if (!data) {
                data = "";
            }
            this.dispatchEvent((observer: ICommandObserver) => {
                observer.onReceived({command: cmd, data: data});
            });
        } catch (e) {
            this._logger.error(`${this.kTag}: onMessageReceived: ${e}`);
        }
    }
}