import { Adjust } from "../adjust/internal/Adjust";
import { AdMob } from "../ad_mob/internal";
import { Facebook } from "../facebook/internal";
import { FacebookAds } from "../facebook_ads/internal/FacebookAds";
import { FirebaseCrashlytics } from "../firebase_crashlytics/internal";
import { FirebasePerformance } from "../firebase_performance/internal";
import { IronSource } from "../iron_source/internal";
import { Notification } from "../notification/internal";
import { UnityAds } from "../unity_ads/internal";
import { ILogger } from "./ILogger";
import { IMessageBridge } from "./IMessageBridge"
import {
    IPluginManagerImpl,
    MessageBridge,
    PluginManagerImplEditor,
    PluginManagerImplNative,
} from "./internal";
import { IPlugin } from "./IPlugin";
import { Logger } from "./Logger";
import { LogLevel } from "./LogLevel";
import { Platform } from "./Platform";

declare const global: any;
declare const CC_JSB: boolean;

const _global = (typeof window === 'undefined' ? global : window) as any;
_global.ee_x = _global.ee_x || {};
_global.ee_x.ee_callCppInternal = (tag: string, message: string) => {
    PluginManager.getBridge().callCpp(tag, message);
}

export enum Plugin {
    Adjust,
    AdMob,
    Facebook,
    FacebookAds,
    FirebaseCrashlytics,
    FirebasePerformance,
    IronSource,
    Notification,
    UnityAds,
}

export class PluginManager {
    private static readonly _pluginInfo: { [index: string]: [string, (bridge: IMessageBridge, logger: ILogger, destroyer: () => void) => IPlugin] } = {
        [Plugin.Adjust]: [`Adjust`, (bridge, logger, destroyer) => new Adjust(bridge, logger, destroyer)],
        [Plugin.AdMob]: [`AdMob`, (bridge, logger, destroyer) => new AdMob(bridge, logger, destroyer)],
        [Plugin.Facebook]: [`Facebook`, (bridge, logger, destroyer) => new Facebook(bridge, logger, destroyer)],
        [Plugin.FacebookAds]: [`FacebookAds`, (bridge, logger, destroyer) => new FacebookAds(bridge, logger, destroyer)],
        [Plugin.FirebaseCrashlytics]: [`FirebaseCrashlytics`, (bridge, logger, destroyer) => new FirebaseCrashlytics(bridge, logger, destroyer)],
        [Plugin.FirebasePerformance]: [`FirebasePerformance`, (bridge, logger, destroyer) => new FirebasePerformance(bridge, logger, destroyer)],
        [Plugin.IronSource]: [`IronSource`, (bridge, logger, destroyer) => new IronSource(bridge, logger, destroyer)],
        [Plugin.Notification]: [`Notification`, (bridge, logger, destroyer) => new Notification(bridge, logger, destroyer)],
        [Plugin.UnityAds]: [`UnityAds`, (bridge, logger, destroyer) => new UnityAds(bridge, logger, destroyer)],
    };

    private static _impl: IPluginManagerImpl;
    private static _logger: Logger;
    private static _bridge: IMessageBridge;

    public static initializePlugins(): void {
        this._logger = new Logger(`ee-x`);
        if (CC_JSB) {
            this._impl = new PluginManagerImplNative();
        } else {
            this._impl = new PluginManagerImplEditor();
        }
        this._bridge = new MessageBridge();
        Platform.initialize(this._bridge);
    }

    public static createPlugin<T extends IPlugin>(plugin: Plugin): T {
        const [name, constructor] = this._pluginInfo[plugin];
        this._impl.addPlugin(name);
        const instance = constructor(this._bridge, this._logger, () => this._impl.removePlugin(name));
        return instance as T;
    }

    public static getBridge(): IMessageBridge {
        return this._bridge;
    }

    public static setLogLevel(level: LogLevel): void {
        this._logger.logLevel = level;
        this._impl.setLogLevel(level);
    }
}