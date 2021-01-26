import {
    FullScreenAdResult,
    IAd,
    IBannerAd,
    IFullScreenAd,
} from "../../ads";
import {
    DefaultFullScreenAd,
    GuardedBannerAd,
    GuardedFullScreenAd,
    IAsyncHelper,
    MediationManager,
} from "../../ads/internal";
import {
    IMessageBridge,
    Utils,
} from "../../core";
import { FacebookBannerAdSize } from "../FacebookBannerAdSize";
import { IFacebookAds } from "../IFacebookAds";
import { FacebookBannerAd } from "./FacebookBannerAd";

export class FacebookAds implements IFacebookAds {
    private readonly kPrefix = "FacebookAdsBridge";
    private readonly kInitialize = this.kPrefix + "Initialize";
    private readonly kGetTestDeviceHash = this.kPrefix + "GetTestDeviceHash";
    private readonly kAddTestDevice = this.kPrefix + "AddTestDevice";
    private readonly kClearTestDevices = this.kPrefix + "ClearTestDevices";
    private readonly kGetBannerAdSize = this.kPrefix + "GetBannerAdSize";
    private readonly kCreateBannerAd = this.kPrefix + "CreateBannerAd";
    private readonly kDestroyBannerAd = this.kPrefix + "DestroyBannerAd";
    private readonly kCreateNativeAd = this.kPrefix + "CreateNativeAd";
    private readonly kDestroyNativeAd = this.kPrefix + "DestroyNativeAd";
    private readonly kCreateInterstitialAd = this.kPrefix + "CreateInterstitialAd";
    private readonly kDestroyInterstitialAd = this.kPrefix + "DestroyInterstitialAd";
    private readonly kCreateRewardedAd = this.kPrefix + "CreateRewardedAd";
    private readonly kDestroyRewardedAd = this.kPrefix + "DestroyRewardedAd";

    private readonly _bridge: IMessageBridge;
    private readonly _ads: { [index: string]: IAd };
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;

    public constructor(bridge: IMessageBridge) {
        this._bridge = bridge;
        this._ads = {};
        this._displayer = MediationManager.getInstance().adDisplayer;
    }

    public destroy(): void {
        for (const id in this._ads) {
            const ad = this._ads[id];
            ad.destroy();
            delete this._ads[id];
        }
    }

    public async initialize(): Promise<boolean> {
        const response = await this._bridge.callAsync(this.kInitialize);
        return Utils.toBool(response);
    }

    public getTestDeviceHash(): string {
        return this._bridge.call(this.kGetTestDeviceHash);
    }

    public addTestDevice(hash: string): void {
        this._bridge.call(this.kAddTestDevice, hash);
    }

    public clearTestDevices(): void {
        this._bridge.call(this.kClearTestDevices);
    }


    private getBannerAdSize(adSize: FacebookBannerAdSize): [number, number] {
        const response = this._bridge.call(this.kGetBannerAdSize, `${adSize}`);
        const json: {
            width: number,
            height: number,
        } = JSON.parse(response);
        return [json.width, json.height];
    }

    public createBannerAd(adId: string, adSize: FacebookBannerAdSize): IBannerAd {
        if (this._ads[adId]) {
            return this._ads[adId] as IBannerAd;
        }
        const request = {
            [`adId`]: adId,
            [`adSize`]: parseInt(`${adSize}`),
        };
        const response = this._bridge.call(this.kCreateBannerAd, JSON.stringify(request));
        if (!Utils.toBool(response)) {
            throw new Error(`Failed to create banner ad: ${adId}`);
        }
        const size = this.getBannerAdSize(adSize);
        const ad = new GuardedBannerAd(new FacebookBannerAd(this._bridge, this, adId, size));
        this._ads[adId] = ad;
        return ad;
    }

    public createInterstitialAd(adId: string): IFullScreenAd {
        return this.createFullScreenAd(this.kCreateInterstitialAd, adId, () =>
            new DefaultFullScreenAd("FacebookInterstitialAd", this._bridge, this._displayer,
                () => this.destroyAd(this.kDestroyInterstitialAd, adId),
                _ => FullScreenAdResult.Completed,
                adId))
    }

    public createRewardedAd(adId: string): IFullScreenAd {
        return this.createFullScreenAd(this.kCreateRewardedAd, adId, () =>
            new DefaultFullScreenAd("FacebookRewardedAd", this._bridge, this._displayer,
                () => this.destroyAd(this.kDestroyRewardedAd, adId),
                message => Utils.toBool(message)
                    ? FullScreenAdResult.Completed
                    : FullScreenAdResult.Canceled,
                adId))
    }

    private createFullScreenAd(handlerId: string, adId: string, creator: () => IFullScreenAd): IFullScreenAd {
        if (this._ads[adId]) {
            return this._ads[adId] as IFullScreenAd;
        }
        const response = this._bridge.call(handlerId, adId);
        if (!Utils.toBool(response)) {
            throw new Error(`Failed to create fullscreen ad: ${adId}`);
        }
        const ad = new GuardedFullScreenAd(creator());
        this._ads[adId] = ad;
        return ad;
    }

    public destroyBannerAd(adId: string): boolean {
        return this.destroyAd(this.kDestroyBannerAd, adId);
    }

    private destroyAd(handlerId: string, adId: string): boolean {
        if (!this._ads[adId]) {
            return false;
        }
        const response = this._bridge.call(handlerId, adId);
        if (!Utils.toBool(response)) {
            // Assert.
            return false;
        }
        delete this._ads[adId];
        return true;
    }
}