import { ILogger } from "./ILogger";
import { LogLevel } from "./LogLevel";

export class Logger implements ILogger {
    private readonly _tag: string;
    private _logLevel: LogLevel;

    public constructor(tag: string) {
        this._tag = tag;
        this._logLevel = LogLevel.Verbose;
    }

    public get logLevel(): LogLevel {
        return this._logLevel;
    }

    public set logLevel(value: LogLevel) {
        this._logLevel = value;
    }

    public error(message: string): void {
        if (this._logLevel <= LogLevel.Error) {
            console.error(`${this._tag}: ${message}`);
        }
    }

    public warn(message: string): void {
        if (this._logLevel <= LogLevel.Warning) {
            console.warn(`${this._tag}: ${message}`);
        }
    }

    public info(message: string): void {
        if (this._logLevel <= LogLevel.Info) {
            console.info(`${this._tag}: ${message}`);
        }
    }

    public debug(message: string): void {
        if (this._logLevel <= LogLevel.Debug) {
            console.debug(`${this._tag}: ${message}`);
        }
    }
}