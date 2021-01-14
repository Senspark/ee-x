import { LogLevel } from "../LogLevel";

export interface IPluginManagerImpl {
    initializePLugins(version: string): void;
    setLogLevel(level: LogLevel): void;
    addPlugin(name: string): void;
    removePlugin(name: string): void;
}