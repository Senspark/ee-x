import { LogLevel } from "../LogLevel";
import { IPluginManagerImpl } from "./IPluginManagerImpl";

declare function ee_staticLogLevel(level: number): void;
declare function ee_staticAddPlugin(name: string): void;
declare function ee_staticRemovePlugin(name: string): void;

export class PluginManagerImplNative implements IPluginManagerImpl {
    public setLogLevel(level: LogLevel): void {
        ee_staticLogLevel(level);
    }

    public addPlugin(name: string): void {
        ee_staticAddPlugin(name);
    }

    public removePlugin(name: string): void {
        ee_staticRemovePlugin(name);
    }
}