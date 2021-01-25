import { LogLevel } from "../LogLevel";
import { Plugin } from '../PluginManager';
import { IPluginManagerImpl } from "./IPluginManagerImpl";

declare function ee_staticLogLevel(level: number): void;
declare function ee_staticAddPlugin(plugin: number): void;
declare function ee_staticRemovePlugin(plugin: number): void;

export class PluginManagerImplNative implements IPluginManagerImpl {
    public setLogLevel(level: LogLevel): void {
        ee_staticLogLevel(level);
    }

    public addPlugin(plugin: Plugin): void {
        ee_staticAddPlugin(plugin);
    }

    public removePlugin(plugin: Plugin): void {
        ee_staticRemovePlugin(plugin);
    }
}