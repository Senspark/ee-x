import {ILogger, IMessageBridge, ObserverManager,} from "../../core";
import {ICommandObserver, ICommandReceiver} from "../ICommandReceiver";

type Destroyer = () => void;

export class CommandReceiver extends ObserverManager<ICommandObserver> implements ICommandReceiver {
    private readonly kTag = `CommandReceiver`;
    private readonly kPrefix = `${this.kTag}Bridge`;
    private readonly kOnMessageReceived = `${this.kPrefix}OnMessageReceived`;

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

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