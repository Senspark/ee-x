import { IAd } from "../../ads/IAd";
import { IBannerAd } from "../../ads/IBannerAd";
import { FullScreenAdResult, IFullScreenAd } from "../../ads/IFullScreenAd";
import { GuardedBannerAd } from "../../ads/internal/GuardedBannerAd";
import { GuardedFullScreenAd } from "../../ads/internal/GuardedFullScreenAd";
import { IAsyncHelper } from "../../ads/internal/IAsyncHelper";
import { MediationManager } from "../../ads/internal/MediationManager";
import { IMessageBridge } from "../../core/IMessageBridge";
import { Utils } from "../../core/Utils";
import { AdMobBannerAdSize } from "../AdMobBannerAdSIze";
import { IAdMob } from "../IAdMob";
import { AdMobBannerAd } from "./AdMobBannerAd";
import { AdMobInterstitialAd } from "./AdMobInterstitialAd";
import { AdMobRewardedAd } from "./AdMobRewardedAd";

export class AdMob implements IAdMob {
    private readonly kPrefix = "AdMobBridge";
    private readonly kInitialize = this.kPrefix + "Initialize";
    private readonly kGetEmulatorTestDeviceHash = this.kPrefix + "GetEmulatorTestDeviceHash";
    private readonly kAddTestDevice = this.kPrefix + "AddTestDevice";
    private readonly kGetBannerAdSize = this.kPrefix + "GetBannerAdSize";
    private readonly kCreateBannerAd = this.kPrefix + "CreateBannerAd";
    private readonly kDestroyBannerAd = this.kPrefix + "DestroyBannerAd";
    private readonly kCreateNativeAd = this.kPrefix + "CreateNativeAd";
    private readonly kDestroyNativeAd = this.kPrefix + "DestroyNativeAd";
    private readonly kCreateAppOpenAd = this.kPrefix + "CreateAppOpenAd";
    private readonly kDestroyAppOpenAd = this.kPrefix + "DestroyAppOpenAd";
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

    public getEmulatorTestDeviceHash(): string {
        return this._bridge.call(this.kGetEmulatorTestDeviceHash);
    }

    public addTestDevice(hash: string): void {
        this._bridge.call(this.kAddTestDevice, hash);
    }

    private getBannerAdSize(adSize: AdMobBannerAdSize): [number, number] {
        const response = this._bridge.call(this.kGetBannerAdSize, `${adSize}`);
        var json: {
            width: number,
            height: number,
        } = JSON.parse(response);
        return [json.width, json.height];
    }


    public createBannerAd(adId: string, adSize: AdMobBannerAdSize): IBannerAd {
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
        const ad = new GuardedBannerAd(new AdMobBannerAd(this._bridge, this, adId, size));
        this._ads[adId] = ad;
        return ad;
    }

    public createAppOpenAd(adId: string): IFullScreenAd {
        // FIXME.
        throw new Error(`Not supported`);
        // return this.createFullScreenAd(this.kCreateAppOpenAd, adId,
        //     () => new AdMobAppOpenAd(this._bridge, this._displayer, this, adId));
    }

    public createInterstitialAd(adId: string): IFullScreenAd {
        return this.createFullScreenAd(this.kCreateInterstitialAd, adId,
            () => new AdMobInterstitialAd(this._bridge, this._displayer, this, adId));
    }

    public createRewardedAd(adId: string): IFullScreenAd {
        return this.createFullScreenAd(this.kCreateRewardedAd, adId,
            () => new AdMobRewardedAd(this._bridge, this._displayer, this, adId));
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

    public destroyAppOpenAd(adId: string): boolean {
        return this.destroyAd(this.kDestroyAppOpenAd, adId);
    }

    public destroyInterstitialAd(adId: string): boolean {
        return this.destroyAd(this.kDestroyInterstitialAd, adId);
    }

    public destroyRewardedAd(adId: string): boolean {
        return this.destroyAd(this.kDestroyRewardedAd, adId);
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