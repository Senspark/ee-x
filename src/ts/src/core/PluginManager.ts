import { Adjust } from "../adjust/internal/Adjust";
import { AdMob } from "../ad_mob/internal";
import { FacebookAds } from "../facebook_ads/internal/FacebookAds";
import { FirebaseCrashlytics } from "../firebase_crashlytics/internal";
import { FirebasePerformance } from "../firebase_performance/internal";
import { IronSource } from "../iron_source/internal";
import { Notification } from "../notification/internal";
import { UnityAds } from "../unity_ads/internal";
import { IMessageBridge } from "./IMessageBridge"
import {
    IPluginManagerImpl,
    MessageBridge,
    PluginManagerImplNative,
} from "./internal";
import { IPlugin } from "./IPlugin";
import { LogLevel } from "./LogLevel";
import { Platform } from "./Platform";

declare const global: any;

const _global = (typeof window === 'undefined' ? global : window) as any;
_global.ee_x = _global.ee_x || {};
_global.ee_x.ee_callCppInternal = (tag: string, message: string) => {
    PluginManager.getBridge().callCpp(tag, message);
}

export enum Plugin {
    Adjust = 1,
    AdMob,
    FacebookAds = 6,
    FirebaseCrashlytics = 8,
    FirebasePerformance,
    IronSource = 11,
    Notification,
    UnityAds = 16,
}

export class PluginManager {
    private static readonly _pluginConstructors: { [index: string]: (bridge: IMessageBridge) => IPlugin } = {
        [Plugin.Adjust]: bridge => new Adjust(bridge),
        [Plugin.AdMob]: bridge => new AdMob(bridge),
        [Plugin.FacebookAds]: bridge => new FacebookAds(bridge),
        [Plugin.FirebaseCrashlytics]: bridge => new FirebaseCrashlytics(bridge),
        [Plugin.FirebasePerformance]: bridge => new FirebasePerformance(bridge),
        [Plugin.IronSource]: bridge => new IronSource(bridge),
        [Plugin.Notification]: bridge => new Notification(bridge),
        [Plugin.UnityAds]: bridge => new UnityAds(bridge),
    };

    private static readonly _plugins: { [index: string]: IPlugin } = {};

    private static _impl: IPluginManagerImpl;
    private static _bridge: IMessageBridge;

    public static initializePlugins(): void {
        this._impl = new PluginManagerImplNative();
        this._bridge = new MessageBridge();
        Platform.initialize(this._bridge);
    }

    public static createPlugin<T extends IPlugin>(plugin: Plugin): T {
        const constructor = this._pluginConstructors[plugin];
        this.addPlugin(plugin);
        const instance = constructor(this._bridge);
        this._plugins[plugin] = instance;
        return instance as T;
    }

    public static destroyPlugin(plugin: Plugin): void {
        this.removePlugin(plugin);
        delete this._plugins[plugin];
    }

    public static getBridge(): IMessageBridge {
        return this._bridge;
    }

    public static setLogLevel(level: LogLevel): void {
        this._impl.setLogLevel(level);
    }

    private static addPlugin(plugin: Plugin): void {
        this._impl.addPlugin(plugin);
    }

    private static removePlugin(plugin: Plugin): void {
        this._impl.removePlugin(plugin);
    }
}