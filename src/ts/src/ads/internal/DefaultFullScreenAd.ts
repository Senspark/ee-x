import {
    ILogger,
    IMessageBridge,
    ObserverManager,
    Utils,
} from "../../core";
import { AdObserver } from "../IAd";
import {
    FullScreenAdResult,
    IFullScreenAd,
} from "../IFullScreenAd";
import { AsyncHelper } from "./AsyncHelper";
import { IAsyncHelper } from "./IAsyncHelper";
import { MessageHelper } from "./MessageHelper";

type Destroyer = () => void;
type ResultParser = (message: string) => FullScreenAdResult;

export class DefaultFullScreenAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly kTag = `DefaultFullScreenAd`;
    private readonly _prefix: string;
    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;
    private readonly _destroyer: Destroyer;
    private readonly _resultParser: ResultParser;
    private readonly _adId: string;
    private readonly _messageHelper: MessageHelper;
    private _loadingCapped: boolean;
    private readonly _loader: IAsyncHelper<boolean>;

    public constructor(
        prefix: string,
        bridge: IMessageBridge,
        logger: ILogger,
        displayer: IAsyncHelper<FullScreenAdResult>,
        destroyer: Destroyer,
        resultParser: ResultParser,
        adId: string) {
        super();
        this._prefix = prefix;
        this._bridge = bridge;
        this._logger = logger;
        this._displayer = displayer;
        this._destroyer = destroyer;
        this._resultParser = resultParser;
        this._adId = adId;
        this._messageHelper = new MessageHelper(prefix, adId);
        this._loadingCapped = false;
        this._loader = new AsyncHelper<boolean>();

        this._logger.debug(`${this.kTag}: constructor: prefix = ${this._prefix} id = ${this._adId}`);
        this._bridge.registerHandler(_ => this.onLoaded(), this._messageHelper.onLoaded);
        this._bridge.registerHandler(this.onFailedToLoad, this._messageHelper.onFailedToLoad);
        this._bridge.registerHandler(this.onFailedToShow, this._messageHelper.onFailedToShow);
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
        if (this._loadingCapped) {
            return false;
        }
        this._loadingCapped = true;
        Utils.noAwait(async () => {
            await Utils.delay(30);
            this._loadingCapped = false;
        });
        return await this._loader.process(
            () => this._bridge.call(this._messageHelper.load),
            result => {
                // OK.
            });
    }

    public show(): Promise<FullScreenAdResult> {
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
        } else {
            // Assert.
        }
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    private onFailedToLoad(message: string): void {
        this._logger.debug(`${this.kTag}: onFailedToLoad: prefix = ${this._prefix} id = ${this._adId} loading = ${this._loader.isProcessing} message = ${message}`);
        if (this._loader.isProcessing) {
            this._loader.resolve(false);
        } else {
            // Assert.
        }
    }

    private onFailedToShow(message: string): void {
        this._logger.debug(`${this.kTag}: onFailedToLoad: prefix = ${this._prefix} id = ${this._adId} displaying = ${this._displayer.isProcessing} message = ${message}`);
        if (this._displayer.isProcessing) {
            this._displayer.resolve(FullScreenAdResult.Failed);
        } else {
            // Assert.
        }
    }

    private onClicked(): void {
        this._logger.debug(`${this.kTag}: onClicked: prefix = ${this._prefix} id = ${this._adId}`);
        this.dispatchEvent(observer => observer.onClicked && observer.onClicked());
    }

    private onClosed(result: FullScreenAdResult): void {
        this._logger.debug(`${this.kTag}: onClosed: prefix = ${this._prefix} id = ${this._adId} displaying = ${this._displayer.isProcessing}`);
        if (this._displayer.isProcessing) {
            this._displayer.resolve(result);
        } else {
            // Assert..
        }
    }
}