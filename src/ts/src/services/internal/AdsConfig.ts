import {
    IAd,
    IBannerAd,
    IFullScreenAd,
    IMultiAd,
    MultiBannerAd,
    MultiFullScreenAd,
    NullAd,
    NullBannerAd,
    NullFullScreenAd,
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
import {
    IIronSource,
    IronSourceBannerAdSize,
} from "../../iron_source";
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

type AdConfigDef = (BannerConfigDef | RectangleConfigDef | AppOpenConfigDef | InterstitialConfigDef | RewardedInterstitialConfigDef | RewardedConfigDef) & {
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

type RewardedInterstitialConfigDef = {
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

function parseNetwork(id: string): Network {
    switch (id) {
        case "ad_mob": return Network.AdMob;
        case "facebook_ads": return Network.FacebookAds;
        case "iron_source": return Network.IronSource;
        case "unity_ads": return Network.UnityAds;
        default: return Network.Null;
    }
}

function parseAdFormat(id: string): AdFormat {
    switch (id) {
        case "banner": return AdFormat.Banner;
        case "rect": return AdFormat.Rectangle;
        case "app_open": return AdFormat.AppOpen;
        case "interstitial": return AdFormat.Interstitial;
        case "rewarded_interstitial": return AdFormat.RewardedInterstitial;
        case "rewarded": return AdFormat.Rewarded;
        default: return AdFormat.Null;
    }
}

export enum Network {
    AdMob,
    FacebookAds,
    IronSource,
    UnityAds,
    Null,
}

export enum AdFormat {
    Banner,
    Rectangle,
    AppOpen,
    Interstitial,
    RewardedInterstitial,
    Rewarded,
    Null,
}

interface INetworkConfigManager {
    initialize(): Promise<void>;
    openTestSuite(network: Network): void;
    createAd(network: Network, format: AdFormat, id: string): IAd;
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

    public openTestSuite(network: Network): void {
        for (const item of this._networks) {
            if (item.network === network) {
                item.openTestSuite();
                break;
            }
        }
    }

    public createAd(network: Network, format: AdFormat, id: string): IAd {
        for (const item of this._networks) {
            if (item.network === network) {
                return item.createAd(format, id);
            }
        }
        return new NullAd();
    }
}

interface INetworkConfig {
    initialize(): Promise<void>;
    network: Network;
    openTestSuite(): void;
    createAd(format: AdFormat, id: string): IAd;
}

class NetworkConfig {
    public static parse(node: NetworkConfigDef): INetworkConfig {
        const network = parseNetwork(node.network);
        switch (network) {
            case Network.AdMob: return new AdMobConfig(node);
            case Network.FacebookAds: return new FacebookAdsConfig(node);
            case Network.IronSource: return new IronSourceConfig(node as IronSourceConfigDef);
            case Network.UnityAds: return new UnityAdsConfig(node as UnityAdsConfigDef);
            case Network.Null: return new NullNetworkConfig();
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

    public openTestSuite(): void {
        this._plugin?.openTestSuite();
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
            case AdFormat.RewardedInterstitial:
                return this._plugin.createRewardedInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            case AdFormat.Null:
                return new NullAd();
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

    public openTestSuite(): void {
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
            case AdFormat.AppOpen:
            case AdFormat.RewardedInterstitial:
                return new NullFullScreenAd();
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            case AdFormat.Null:
                return new NullAd();
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

    public openTestSuite(): void {
    }

    public createAd(format: AdFormat, id: string): IAd {
        if (this._plugin === undefined) {
            throw Error(`Plugin not initialized`);
        }
        switch (format) {
            case AdFormat.Banner:
                return this._plugin.createBannerAd(id, IronSourceBannerAdSize.Banner);
            case AdFormat.Rectangle:
                return this._plugin.createBannerAd(id, IronSourceBannerAdSize.Rectangle);
            case AdFormat.AppOpen:
            case AdFormat.RewardedInterstitial:
                return new NullFullScreenAd();
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            case AdFormat.Null:
                return new NullAd();
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

    public openTestSuite(): void {
    }

    public createAd(format: AdFormat, id: string): IAd {
        if (this._plugin === undefined) {
            throw Error(`Plugin not initialized`);
        }
        switch (format) {
            case AdFormat.Banner:
            case AdFormat.Rectangle:
                return new NullBannerAd();
            case AdFormat.AppOpen:
            case AdFormat.RewardedInterstitial:
                return new NullFullScreenAd();
            case AdFormat.Interstitial:
                return this._plugin.createInterstitialAd(id);
            case AdFormat.Rewarded:
                return this._plugin.createRewardedAd(id);
            case AdFormat.Null:
                return new NullAd();
        }
    }
}

class NullNetworkConfig implements INetworkConfig {
    public async initialize(): Promise<void> {
    }

    public get network(): Network {
        return Network.Null;
    }

    public openTestSuite(): void {
    }

    public createAd(format: AdFormat, id: string): IAd {
        switch (format) {
            case AdFormat.Banner:
            case AdFormat.Rectangle:
                return new NullBannerAd();
            case AdFormat.AppOpen:
            case AdFormat.RewardedInterstitial:
            case AdFormat.Interstitial:
            case AdFormat.Rewarded:
                return new NullFullScreenAd();
            case AdFormat.Null:
                return new NullAd();
        }
    }
}

interface IAdConfigManager {
    createAd(format: AdFormat): IAd;
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
        return new NullAd();
    }
}

interface IAdConfig {
    format: AdFormat;
    createAd(manager: INetworkConfigManager): IAd;
}

class AdConfig {
    public static parse(node: AdConfigDef): IAdConfig {
        const format = parseAdFormat(node.format);
        switch (format) {
            case AdFormat.Banner: return new BannerConfig(node);
            case AdFormat.Rectangle: return new RectangleConfig(node);
            case AdFormat.AppOpen: return new AppOpenConfig(node);
            case AdFormat.Interstitial: return new InterstitialConfig(node);
            case AdFormat.RewardedInterstitial: return new RewardedInterstitialConfig(node);
            case AdFormat.Rewarded: return new RewardedConfig(node);
            case AdFormat.Null: return new NullAdConfig();
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

class RewardedInterstitialConfig implements IAdConfig {
    private readonly _interval: number;
    private readonly _instance: IAdInstanceConfig<IFullScreenAd>;

    public constructor(node: RewardedInterstitialConfigDef) {
        this._interval = node.interval !== undefined ? node.interval : 0;
        this._instance = new AdInstanceConfig<IFullScreenAd, MultiFullScreenAd>()
            .parse(MultiFullScreenAd, AdFormat.RewardedInterstitial, node.instance);
    }

    public get format(): AdFormat {
        return AdFormat.RewardedInterstitial;
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

class NullAdConfig implements IAdConfig {
    public get format(): AdFormat {
        return AdFormat.Null;
    }

    public createAd(manager: INetworkConfigManager): IAd {
        return new NullAd();
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
        this._network = parseNetwork(node.network);
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

    public openTestSuite(network: Network): void {
        this._networkManager.openTestSuite(network);
    }

    public createAd(format: AdFormat): IAd {
        return this._adManager.createAd(format);
    }
}