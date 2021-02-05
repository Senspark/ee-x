import { LogLevel } from "../LogLevel";

export interface IPluginManagerImpl {
    setLogLevel(level: LogLevel): void;
    addPlugin(name: string): void;
    removePlugin(name: string): void;
}