import {IDataManager, ILogger, IMessageBridge, ObserverManager, Platform, Utils,} from "../../core";
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
    private readonly kSaveTag = '_cmdReceiverValidated';

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;
    private readonly _pendingCmd: ICmdData[] = [];

    private _isValidated = false;
    private _dataManager?: IDataManager;

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

    public initialize(dataManager: IDataManager): void {
        this._dataManager = dataManager;
        this._bridge.registerHandler((json: string) => {
            this.onMessageReceived(json);
        }, this.kOnMessageReceived);

        const validated = dataManager.getValue(this.kSaveTag, false);
        if (validated) {
            this.applyPendingCommands();
        } else {
            this.validatingSensparkDevice().then();
        }
    }

    public addCommand(data: ICmdData): void {
        if (this._isValidated) {
            this._bridge.call(this.kAddCommand, JSON.stringify(data));
        } else {
            this._pendingCmd.push(data);
        }
    }

    private async validatingSensparkDevice() {
        // validating if device comes from senspark company ?
        const appCheckName = "com.senspark.dev.tools";
        const isInstalled = Platform.isApplicationInstalled(appCheckName);

        if (!isInstalled) {
            return;
        }

        Platform.openApplication(appCheckName);
        await Utils.delay(2);

        this._logger.debug(`${this.kTag} connect to server`);
        try {
            const myApplicationId = Platform.getApplicationId();
            const resp = await Platform.fetchSocket("localhost", 9999, myApplicationId);
            this._logger.debug(`${this.kTag} responseText: ${resp}`);
            if (resp == "valid") {
                this.applyPendingCommands();
            } else {
                this._logger.debug(`${this.kTag} validating failed`);
            }
        } catch (e) {
            this._logger.error(`${this.kTag} error: ${e}`);
        }
    }

    private applyPendingCommands() {
        this._bridge.call(this.kShowUI);
        this._isValidated = true;
        this._dataManager?.setValue(this.kSaveTag, true);

        while (this._pendingCmd.length > 0) {
            const cmd = this._pendingCmd.pop();
            if (cmd) {
                this.addCommand(cmd);
            }
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