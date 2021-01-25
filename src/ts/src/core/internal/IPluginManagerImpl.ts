import { LogLevel } from "../LogLevel";
import { Plugin } from '../PluginManager';

export interface IPluginManagerImpl {
    setLogLevel(level: LogLevel): void;
    addPlugin(plugin: Plugin): void;
    removePlugin(plugin: Plugin): void;
}