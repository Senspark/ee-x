import {
    ILogger,
    IMessageBridge,
    ObserverManager,
} from "../../core";
import { AdObserver } from "../IAd";
import { IBannerAd } from "../IBannerAd";
import { AsyncHelper } from "./AsyncHelper";
import { BannerAdHelper } from "./BannerAdHelper";
import { Capper } from "./Capper";
import { IAsyncHelper } from "./IAsyncHelper";
import { ICapper } from "./ICapper";
import { MessageHelper } from "./MessageHelper";

type Destroyer = () => void;

export class DefaultBannerAd extends ObserverManager<AdObserver> implements IBannerAd {
    private readonly kTag = `DefaultBannerAd`;
    private readonly _prefix: string;
    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;
    private readonly _network: string;
    private readonly _adId: string;
    private readonly _messageHelper: MessageHelper;
    private readonly _helper: BannerAdHelper;
    private readonly _loadCapper: ICapper;
    private readonly _loader: IAsyncHelper<boolean>;

    public constructor(
        prefix: string,
        bridge: IMessageBridge,
        logger: ILogger,
        destroyer: Destroyer,
        network: string,
        adId: string,
        size: [number, number]) {
        super();
        this._prefix = prefix;
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
        this._network = network;
        this._adId = adId;
        this._messageHelper = new MessageHelper(prefix, adId);
        this._helper = new BannerAdHelper(this._bridge, this._messageHelper, size);
        this._loadCapper = new Capper(5);
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
        this._bridge.registerHandler(_ => this.onClicked(), this._messageHelper.onClicked);
    }

    public destroy(): void {
        this._logger.debug(`${this.kTag}: destroy: prefix = ${this._prefix} id = ${this._adId}`);
        this._bridge.deregisterHandler(this._messageHelper.onLoaded);
        this._bridge.deregisterHandler(this._messageHelper.onFailedToLoad);
        this._bridge.deregisterHandler(this._messageHelper.onClicked);
        this._destroyer();
    }

    public get isLoaded(): boolean {
        return this._helper.isLoaded;
    }

    public async load(): Promise<boolean> {
        this._logger.debug(`${this.kTag}: load: prefix = ${this._prefix} id = ${this._adId} loading = ${this._loader.isProcessing}`);
        if (this._loadCapper.isCapped) {
            return false;
        }
        this._loadCapper.cap();
        return await this._loader.process(
            () => this._helper.load(),
            result => {
                // OK.
            });
    }

    public get anchor(): [number, number] {
        return this._helper.anchor;
    }

    public set anchor(value: [number, number]) {
        this._helper.anchor = value;
    }

    public get position(): [number, number] {
        return this._helper.anchor;
    }

    public set position(value: [number, number]) {
        this._helper.position = value;
    }

    public get size(): [number, number] {
        return this._helper.anchor;
    }

    public set size(value: [number, number]) {
        this._helper.size = value;
    }

    public get isVisible(): boolean {
        return this._helper.isVisible;
    }

    public set isVisible(value: boolean) {
        this._helper.isVisible = value;
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
            // Ignored.
        }
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    private onFailedToLoad(code: number, message: string): void {
        this._logger.debug(`${this.kTag}: onFailedToLoad: prefix = ${this._prefix} id = ${this._adId} loading = ${this._loader.isProcessing} message = ${message}`);
        if (this._loader.isProcessing) {
            this._loader.resolve(false);
            this.dispatchEvent(observer => observer.onLoadResult && observer.onLoadResult({
                network: this._network,
                result: false,
                errorCode: code,
                errorMessage: message,
            }));
        } else {
            // Ignored.
        }
    }

    private onClicked(): void {
        this._logger.debug(`${this.kTag}: onClicked: prefix = ${this._prefix} id = ${this._adId}`);
        this.dispatchEvent(observer => observer.onClicked && observer.onClicked());
    }
}