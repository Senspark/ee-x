import {
    ILogger,
    IMessageBridge,
    ObserverManager,
    Utils,
} from "../../core";
import { AdObserver } from "../IAd";
import {
    AdResult,
    IFullScreenAd,
} from "../IFullScreenAd";
import { AsyncHelper } from "./AsyncHelper";
import { Capper } from "./Capper";
import { IAsyncHelper } from "./IAsyncHelper";
import { ICapper } from "./ICapper";
import { MessageHelper } from "./MessageHelper";

type Destroyer = () => void;
type ResultParser = (message: string) => AdResult;

export class DefaultFullScreenAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly kTag = `DefaultFullScreenAd`;
    private readonly _prefix: string;
    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _displayer: IAsyncHelper<AdResult>;
    private readonly _destroyer: Destroyer;
    private readonly _resultParser: ResultParser;
    private readonly _network: string;
    private readonly _adId: string;
    private readonly _messageHelper: MessageHelper;
    private readonly _loadCapper: ICapper;
    private readonly _loader: IAsyncHelper<boolean>;

    public constructor(
        prefix: string,
        bridge: IMessageBridge,
        logger: ILogger,
        displayer: IAsyncHelper<AdResult>,
        destroyer: Destroyer,
        resultParser: ResultParser,
        network: string,
        adId: string) {
        super();
        this._prefix = prefix;
        this._bridge = bridge;
        this._logger = logger;
        this._displayer = displayer;
        this._destroyer = destroyer;
        this._resultParser = resultParser;
        this._network = network;
        this._adId = adId;
        this._messageHelper = new MessageHelper(prefix, adId);
        this._loadCapper = new Capper(30);
        this._loader = new AsyncHelper<boolean>();

        this._logger.debug(`${this.kTag}: constructor: prefix = ${this._prefix} id = ${this._adId}`);
        this._bridge.registerHandler(_ => this.onLoaded(), this._messageHelper.onLoaded);
        this._bridge.registerHandler(message => {
            const json: {
                code: number,
                message: string,
            } = JSON.parse(message);
            this.onFailedToLoad(json.code, json.message);
        }, this._messageHelper.onFailedToLoad);
        this._bridge.registerHandler(message => {
            const json: {
                code: number,
                message: string,
            } = JSON.parse(message);
            this.onFailedToShow(json.code, json.message);
        }, this._messageHelper.onFailedToShow);
        this._bridge.registerHandler(_ => this.onClicked(), this._messageHelper.onClicked);
        this._bridge.registerHandler(message => {
            const result = this._resultParser(message);
            this.onClosed(result);
        }, this._messageHelper.onClosed);
    }

    public destroy(): void {
        this._logger.debug(`${this.kTag}: destroy: prefix = ${this._prefix} id = ${this._adId}`);
        this._bridge.deregisterHandler(this._messageHelper.onLoaded);
        this._bridge.deregisterHandler(this._messageHelper.onFailedToLoad);
        this._bridge.deregisterHandler(this._messageHelper.onFailedToShow);
        this._bridge.deregisterHandler(this._messageHelper.onClicked);
        this._bridge.deregisterHandler(this._messageHelper.onClosed);
        this._destroyer();
    }

    public get isLoaded(): boolean {
        const response = this._bridge.call(this._messageHelper.isLoaded);
        return Utils.toBool(response);
    }

    public async load(): Promise<boolean> {
        this._logger.debug(`${this.kTag}: load: prefix = ${this._prefix} id = ${this._adId} loading = ${this._loader.isProcessing}`);
        if (this._loadCapper.isCapped) {
            return false;
        }
        this._loadCapper.cap();
        return await this._loader.process(
            () => this._bridge.call(this._messageHelper.load),
            result => {
                // OK.
            });
    }

    public show(): Promise<AdResult> {
        this._logger.debug(`${this.kTag}: show: prefix = ${this._prefix} id = ${this._adId} displaying = ${this._displayer.isProcessing}`);
        return this._displayer.process(
            () => this._bridge.call(this._messageHelper.show),
            result => {
                // OK.
            });
    }

    private onLoaded(): void {
        this._logger.debug(`${this.kTag}: onLoaded: prefix = ${this._prefix} id = ${this._adId} loading = ${this._loader.isProcessing}`);
        if (this._loader.isProcessing) {
            this._loader.resolve(true);
            this.dispatchEvent(observer => observer.onLoadResult && observer.onLoadResult({
                network: this._network,
                result: true,
                errorCode: 0,
                errorMessage: ``,
            }));
        } else {
            // Assert.
        }
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    private onFailedToLoad(code: number, message: string): void {
        this._logger.debug(`${this.kTag}: onFailedToLoad: prefix = ${this._prefix} id = ${this._adId} loading = ${this._loader.isProcessing} message = ${message}`);
        if (this._loader.isProcessing) {
            this._loader.resolve(false);
            this.dispatchEvent(observer => observer.onLoadResult && observer.onLoadResult({
                network: this._network,
                result: true,
                errorCode: code,
                errorMessage: message,
            }));
        } else {
            // Assert.
        }
    }

    private onFailedToShow(code: number, message: string): void {
        this._logger.debug(`${this.kTag}: onFailedToLoad: prefix = ${this._prefix} id = ${this._adId} displaying = ${this._displayer.isProcessing} message = ${message}`);
        if (this._displayer.isProcessing) {
            this._displayer.resolve(AdResult.Failed);
        } else {
            // Assert.
        }
    }

    private onClicked(): void {
        this._logger.debug(`${this.kTag}: onClicked: prefix = ${this._prefix} id = ${this._adId}`);
        this.dispatchEvent(observer => observer.onClicked && observer.onClicked());
    }

    private onClosed(result: AdResult): void {
        this._logger.debug(`${this.kTag}: onClosed: prefix = ${this._prefix} id = ${this._adId} displaying = ${this._displayer.isProcessing}`);
        if (this._displayer.isProcessing) {
            this._displayer.resolve(result);
        } else {
            // Assert..
        }
    }
}