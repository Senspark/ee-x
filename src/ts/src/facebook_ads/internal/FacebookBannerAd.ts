import {
    AdObserver,
    IBannerAd,
} from "../../ads";
import {
    AsyncHelper,
    BannerAdHelper,
    IAsyncHelper,
    MessageHelper,
} from "../../ads/internal";
import {
    IMessageBridge,
    ObserverManager,
} from "../../core";
import { FacebookAds } from "./FacebookAds";

export class FacebookBannerAd extends ObserverManager<AdObserver> implements IBannerAd {
    private readonly _bridge: IMessageBridge;
    private readonly _plugin: FacebookAds;
    private readonly _adId: string;
    private readonly _messageHelper: MessageHelper;
    private readonly _helper: BannerAdHelper;
    private readonly _loader: IAsyncHelper<boolean>;

    public constructor(
        bridge: IMessageBridge, plugin: FacebookAds, adId: string, size: [number, number]) {
        super();
        this._bridge = bridge;
        this._plugin = plugin;
        this._adId = adId;
        this._messageHelper = new MessageHelper("FacebookBannerAd", adId);
        this._helper = new BannerAdHelper(this._bridge, this._messageHelper, size);
        this._loader = new AsyncHelper<boolean>();

        this._bridge.registerHandler(_ => this.onLoaded(), this._messageHelper.onLoaded);
        this._bridge.registerHandler(this.onFailedToLoad, this._messageHelper.onFailedToLoad);
        this._bridge.registerHandler(_ => this.onClicked(), this._messageHelper.onClicked);
    }

    public destroy(): void {
        this._bridge.deregisterHandler(this._messageHelper.onLoaded);
        this._bridge.deregisterHandler(this._messageHelper.onFailedToLoad);
        this._bridge.deregisterHandler(this._messageHelper.onClicked);
        this._plugin.destroyBannerAd(this._adId);
    }

    public get isLoaded(): boolean {
        return this._helper.isLoaded;
    }

    public load(): Promise<boolean> {
        return this._loader.process(
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
        if (this._loader.isProcessing) {
            this._loader.resolve(true);
        } else {
            // Ignored.
        }
        this.dispatchEvent(observer => observer.onLoaded && observer.onLoaded());
    }

    private onFailedToLoad(message: string): void {
        if (this._loader.isProcessing) {
            this._loader.resolve(false);
        } else {
            // Ignored.
        }
    }

    private onClicked(): void {
        this.dispatchEvent(observer => observer.onClicked && observer.onClicked());
    }
}