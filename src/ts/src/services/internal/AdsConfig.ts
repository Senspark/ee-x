import {
    IAd,
    IBannerAd,
    IFullScreenAd,
    IMultiAd,
    MultiBannerAd,
    MultiFullScreenAd,
} from "../../ads";
import {
    AdMobBannerAdSize,
    IAdMob,
} from "../../ad_mob";
import { CocosBannerAd } from "../../cocos";
import {
    Plugin,
    PluginManager,
    Utils,
} from "../../core";
import {
    FacebookBannerAdSize,
    IFacebookAds,
} from "../../facebook_ads";
import { IIronSource } from "../../iron_source";
import { IUnityAds } from "../../unity_ads";
import { GenericAd } from "./GenericAd";

type AdsConfigDef = {
    networks: NetworkConfigDef[],
    ads: AdConfigDef[],
}

type NetworkConfigDef = (AdMobConfigDef | FacebookAdsConfigDef | IronSourceConfigDef | UnityAdsConfigDef) & {
    network: string,
}

type AdMobConfigDef = {}

type FacebookAdsConfigDef = {}

type IronSourceConfigDef = {
    app_id: string,
}

type UnityAdsConfigDef = {
    app_id: string,
    time_out?: number,
}

type AdConfigDef = (BannerConfigDef | RectangleConfigDef | AppOpenConfigDef | InterstitialConfigDef | RewardedConfigDef) & {
    format: string,
}

type BannerConfigDef = {
    instance: AdInstanceConfigDef,
}

type RectangleConfigDef = {
    instance: AdInstanceConfigDef,
}

type AppOpenConfigDef = {
    interval?: number,
    instance: AdInstanceConfigDef,
}

type InterstitialConfigDef = {
    interval?: number,
    instance: AdInstanceConfigDef,
}

type RewardedConfigDef = {
    instance: AdInstanceConfigDef,
}

type AdInstanceConfigDef = SingleInstanceConfigDef | WaterfallInstanceConfigDef;

type SingleInstanceConfigDef = {
    network: string,
    id: string,
}

type WaterfallInstanceConfigDef = SingleInstanceConfigDef[];

export enum Network {
    AdMob,
    FacebookAds,
    IronSource,
    UnityAds,
}

export enum AdFormat {
    Banner,
    Rectangle,
    AppOpen,
    Interstitial,
    Rewarded,
}

interface INetworkConfigManager {
    initialize(): Promise<void>;
    createAd(network: Network, format: AdFormat, id: string): IAd | undefined;
}

class NetworkConfigManager implements INetworkConfigManager {
    private readonly _networks: INetworkConfig[] = [];

    public constructor(node: AdsConfigDef) {
        for (const value of node.networks) {
            const network = NetworkConfig.parse(value);
            this._networks.push(network);
        }
    }

    public async initialize(): Promise<void> {
        for (const network of this._networks) {
            await network.initialize();
        }
    }

    public createAd(network: Network, format: AdFormat, id: string): IAd | undefined {
        for (const item of this._networks) {
            if (item.network === network) {
                return item.createAd(format, id);
            }
        }
        return undefined;
    }
}

interface INetworkConfig {
    initialize(): Promise<void>;
    network: Network;
    createAd(format: AdFormat, id: string): IAd;
}

class NetworkConfig {
    public static parse(node: NetworkConfigDef): INetworkConfig {
        const network = node.network;
        switch (network) {
            case "ad_mob": return new AdMobConfig(node);
            case "facebook_ads": return new FacebookAdsConfig(node);
            case "iron_source": return new IronSourceConfig(node as IronSourceConfigDef);
            case "unity_ads": return new UnityAdsConfig(node as UnityAdsConfigDef);
            default: throw new Error(`Invalid network`);
        }
    }
}

class AdMobConfig implements INetworkConfig {
    private _plugin?: IAdMob;

    public constructor(node: AdMobConfigDef) {
    }

    public async initialize(): Promise<void> {
        this._plugin = PluginManager.createPlugin<IAdMob>(Plugin.AdMob);
        await this._plugin.initialize();
    }

    public get network(): Network {
        return Network.AdMob;
    }

    public createAd(format: AdFormat, id: string): IAd {
        if (this._plugin === undefined) {
            throw Error(`Plugin not initialized`);
        }
        switch (format) {
            case AdFormat.Banner:
                return this._plugin.createBannerAd(id, AdMobBannerAdSize.Normal);
            case AdFormat.Rectangle:
                return this._plugin.createBannerAd(id, AdMobBannerAdSize.MediumRectangle);
            case AdFormat.AppOpen:
                return this._plugin.createAppOpenAd(id);
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            default:
                throw Error(`Ad format not supported`);
        }
    }
}

class FacebookAdsConfig implements INetworkConfig {
    private _plugin?: IFacebookAds;

    public constructor(node: FacebookAdsConfigDef) {
    }

    public async initialize(): Promise<void> {
        this._plugin = PluginManager.createPlugin<IFacebookAds>(Plugin.FacebookAds);
        await this._plugin.initialize();
    }

    public get network(): Network {
        return Network.FacebookAds;
    }

    public createAd(format: AdFormat, id: string): IAd {
        if (this._plugin === undefined) {
            throw Error(`Plugin not initialized`);
        }
        switch (format) {
            case AdFormat.Banner:
                return this._plugin.createBannerAd(id, FacebookBannerAdSize.BannerHeight50);
            case AdFormat.Rectangle:
                return this._plugin.createBannerAd(id, FacebookBannerAdSize.RectangleHeight250);
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            default:
                throw Error(`Ad format not supported`);
        }
    }
}

class IronSourceConfig implements INetworkConfig {
    private _plugin?: IIronSource;
    private readonly _appId: string;

    public constructor(node: IronSourceConfigDef) {
        this._appId = node.app_id;
    }

    public async initialize(): Promise<void> {
        this._plugin = PluginManager.createPlugin<IIronSource>(Plugin.IronSource);
        await this._plugin.initialize(this._appId);
    }

    public get network(): Network {
        return Network.IronSource;
    }

    public createAd(format: AdFormat, id: string): IAd {
        if (this._plugin === undefined) {
            throw Error(`Plugin not initialized`);
        }
        switch (format) {
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            default:
                throw Error(`Ad format not supported`);
        }
    }
}

class UnityAdsConfig implements INetworkConfig {
    private _plugin?: IUnityAds;
    private readonly _appId: string;
    private readonly _timeOut: number;

    public constructor(node: UnityAdsConfigDef) {
        this._appId = node.app_id;
        this._timeOut = node.time_out !== undefined ? node.time_out : 30;
    }

    public async initialize(): Promise<void> {
        this._plugin = PluginManager.createPlugin<IUnityAds>(Plugin.UnityAds);
        await Promise.race([
            Utils.delay(this._timeOut),
            this._plugin.initialize(this._appId, false),
        ]);
    }

    public get network(): Network {
        return Network.UnityAds;
    }

    public createAd(format: AdFormat, id: string): IAd {
        if (this._plugin === undefined) {
            throw Error(`Plugin not initialized`);
        }
        switch (format) {
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            default:
                throw Error(`Ad format not supported`);
        }
    }
}

interface IAdConfigManager {
    createAd(format: AdFormat): IAd
}

class AdConfigManager implements IAdConfigManager {
    private readonly _manager: INetworkConfigManager;
    private readonly _ads: IAdConfig[] = [];

    public constructor(manager: INetworkConfigManager, node: AdsConfigDef) {
        this._manager = manager;
        for (const value of node.ads) {
            const ad = AdConfig.parse(value);
            this._ads.push(ad);
        }
    }

    public createAd(format: AdFormat): IAd {
        for (const ad of this._ads) {
            if (ad.format == format) {
                return ad.createAd(this._manager);
            }
        }
        throw new Error(`Ad format not supported`);
    }
}

interface IAdConfig {
    format: AdFormat;
    createAd(manager: INetworkConfigManager): IAd;
}

class AdConfig {
    public static parse(node: AdConfigDef): IAdConfig {
        const format = node.format;
        switch (format) {
            case "banner": return new BannerConfig(node);
            case "rect": return new RectangleConfig(node);
            case "app_open": return new AppOpenConfig(node);
            case "interstitial": return new InterstitialConfig(node);
            case "rewarded": return new RewardedConfig(node);
            default:
                throw new Error(`Ad format not supported`);
        }
    }
}

class BannerConfig implements IAdConfig {
    private readonly _instance: IAdInstanceConfig<IBannerAd>;

    public constructor(node: BannerConfigDef) {
        this._instance = new AdInstanceConfig<IBannerAd, MultiBannerAd>()
            .parse(MultiBannerAd, AdFormat.Banner, node.instance);
    }

    public get format(): AdFormat {
        return AdFormat.Banner;
    }

    public createAd(manager: INetworkConfigManager): IAd {
        const ad = this._instance.createAd(manager);
        return new CocosBannerAd(ad);
    }
}

class RectangleConfig implements IAdConfig {
    private readonly _instance: IAdInstanceConfig<IBannerAd>;

    public constructor(node: RectangleConfigDef) {
        this._instance = new AdInstanceConfig<IBannerAd, MultiBannerAd>()
            .parse(MultiBannerAd, AdFormat.Rectangle, node.instance);
    }

    public get format(): AdFormat {
        return AdFormat.Rectangle;
    }

    public createAd(manager: INetworkConfigManager): IAd {
        const ad = this._instance.createAd(manager);
        return new CocosBannerAd(ad);
    }
}

class AppOpenConfig implements IAdConfig {
    private readonly _interval: number;
    private readonly _instance: IAdInstanceConfig<IFullScreenAd>;

    public constructor(node: AppOpenConfigDef) {
        this._interval = node.interval !== undefined ? node.interval : 0;
        this._instance = new AdInstanceConfig<IFullScreenAd, MultiFullScreenAd>()
            .parse(MultiFullScreenAd, AdFormat.AppOpen, node.instance);
    }

    public get format(): AdFormat {
        return AdFormat.AppOpen;
    }

    public createAd(manager: INetworkConfigManager): IAd {
        const ad = this._instance.createAd(manager);
        return new GenericAd(ad, this._interval);
    }
}

class InterstitialConfig implements IAdConfig {
    private readonly _interval: number;
    private readonly _instance: IAdInstanceConfig<IFullScreenAd>;

    public constructor(node: InterstitialConfigDef) {
        this._interval = node.interval !== undefined ? node.interval : 0;
        this._instance = new AdInstanceConfig<IFullScreenAd, MultiFullScreenAd>()
            .parse(MultiFullScreenAd, AdFormat.Interstitial, node.instance);
    }

    public get format(): AdFormat {
        return AdFormat.Interstitial;
    }

    public createAd(manager: INetworkConfigManager): IAd {
        const ad = this._instance.createAd(manager);
        return new GenericAd(ad, this._interval);
    }
}

class RewardedConfig implements IAdConfig {
    private readonly _instance: IAdInstanceConfig<IFullScreenAd>;

    public constructor(node: RewardedConfigDef) {
        this._instance = new AdInstanceConfig<IFullScreenAd, MultiFullScreenAd>()
            .parse(MultiFullScreenAd, AdFormat.Rewarded, node.instance);
    }

    public get format(): AdFormat {
        return AdFormat.Rewarded;
    }

    public createAd(manager: INetworkConfigManager): IAd {
        const ad = this._instance.createAd(manager);
        return new GenericAd(ad, 0);
    }
}

interface IAdInstanceConfig<Ad extends IAd> {
    createAd(manager: INetworkConfigManager): Ad;
}

class AdInstanceConfig<Ad extends IAd, MultiAd extends IMultiAd<Ad> & Ad> {
    public parse(type: { new(): MultiAd }, format: AdFormat, node: AdInstanceConfigDef): IAdInstanceConfig<Ad> {
        if (node instanceof Array) {
            return new WaterfallInstanceConfig<Ad, MultiAd>(type, format, node);
        }
        return new SingleInstanceConfig<Ad>(format, node);
    }
}

class SingleInstanceConfig<Ad extends IAd> implements IAdInstanceConfig<Ad> {
    private readonly _format: AdFormat;
    private readonly _network: Network;
    private readonly _id: string;

    public constructor(format: AdFormat, node: SingleInstanceConfigDef) {
        this._format = format;
        const networks: {
            [index: string]: Network,
        } = {
            ["ad_mob"]: Network.AdMob,
            ["facebook_ads"]: Network.FacebookAds,
            ["iron_source"]: Network.IronSource,
            ["unity_ads"]: Network.UnityAds
        };
        const network = node.network;
        this._network = networks[network];
        this._id = node[`id`] !== undefined ? node["id"] : "";
    }

    public createAd(manager: INetworkConfigManager): Ad {
        const ad = manager.createAd(this._network, this._format, this._id);
        return ad as Ad;
    }
}

class WaterfallInstanceConfig<Ad extends IAd, MultiAd extends IMultiAd<Ad> & Ad> implements IAdInstanceConfig<Ad> {
    private readonly _instances: IAdInstanceConfig<Ad>[] = [];
    private readonly _type: { new(): MultiAd }

    public constructor(type: { new(): MultiAd }, format: AdFormat, node: WaterfallInstanceConfigDef) {
        this._type = type;
        for (const value of node) {
            this._instances.push(new AdInstanceConfig<Ad, MultiAd>().parse(type, format, value));
        }
    }

    public createAd(manager: INetworkConfigManager): Ad {
        const ad = new this._type();
        for (const instance of this._instances) {
            ad.addItem(instance.createAd(manager));
        }
        return ad;
    }
}

export class AdsConfig {
    private _networkManager: INetworkConfigManager;
    private _adManager: IAdConfigManager;

    public static parse(text: string): AdsConfig {
        const node: AdsConfigDef = JSON.parse(text);
        return this.parseDef(node);
    }

    private static parseDef(node: AdsConfigDef): AdsConfig {
        const networkManager = new NetworkConfigManager(node);
        const adManager = new AdConfigManager(networkManager, node);
        const result = new AdsConfig(networkManager, adManager);
        return result;
    }

    private constructor(networkManager: INetworkConfigManager, adManager: IAdConfigManager) {
        this._networkManager = networkManager;
        this._adManager = adManager;
    }

    public async initialize(): Promise<void> {
        await this._networkManager.initialize();
    }

    public createAd(format: AdFormat): IAd {
        return this._adManager.createAd(format);
    }
}