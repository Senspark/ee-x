import { IMessageBridge } from "./IMessageBridge"
import { IPluginManagerImpl } from "./internal/IPluginManagerImpl";
import { MessageBridge } from "./internal/MessageBridge";
import { IPlugin } from "./IPlugin";
import { LogLevel } from "./LogLevel";
import { Platform } from "./Platform";

export enum Plugin {
    Adjust,
    AdMob,
    FacebookAds,
    FirebaseCrashlytics,
    IronSource,
    UnityAds,
}

export class PluginManager {
    private static readonly _pluginNames: { [index: string]: string } = {
        [Plugin.Adjust]: `Adjust`,
        [Plugin.AdMob]: `AdMob`,
        [Plugin.FacebookAds]: `FacebookAds`,
        [Plugin.FirebaseCrashlytics]: `FirebaseCrashlytics`,
        [Plugin.IronSource]: `IronSource`,
        [Plugin.UnityAds]: `UnityAds`,
    };

    private static readonly _pluginConstructores: { [index: string]: (bridge: IMessageBridge) => IPlugin } = {
    };

    private static readonly _plugins: { [index: string]: IPlugin } = {};

    private static readonly _impl: IPluginManagerImpl;
    private static _bridge: IMessageBridge;

    public static initializePlugins(): void {
        this._impl.initializePLugins("2.3.0");
        this._bridge = MessageBridge.getInstance();
        Platform.registerHandlers(this._bridge);
    }

    public static createPlugin(plugin: Plugin): IPlugin {
        const constructor = this._pluginConstructores[plugin];
        this.addPlugin(plugin);
        const instance = constructor(this._bridge);
        this._plugins[plugin] = instance;
        return instance;
    }

    public static destroyPlugin(plugin: Plugin): void {
        this.removePlugin(plugin);
        delete this._plugins[plugin];
    }

    public static setLogLevel(level: LogLevel): void {
        this._impl.setLogLevel(level);
    }

    private static addPlugin(plugin: Plugin): void {
        const name = this._pluginNames[plugin];
        this._impl.addPlugin(name);
    }

    private static removePlugin(plugin: Plugin): void {
        const name = this._pluginNames[plugin];
        this._impl.removePlugin(name);
    }
}