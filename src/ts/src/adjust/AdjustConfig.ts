import { AdjustEnvironment } from "./AdjustEnvironment";
import { AdjustLogLevel } from "./AdjustLogLevel";

export class AdjustConfig {
    private _token = ``;
    private _environment = AdjustEnvironment.Sandbox;
    private _logLevel = AdjustLogLevel.Verbose;
    private _eventBufferingEnabled = false;

    public get token(): string {
        return this._token;
    }

    public set token(value: string) {
        this._token = value;
    }

    public get environment(): AdjustEnvironment {
        return this._environment;
    }

    public set environment(value: AdjustEnvironment) {
        this._environment = value;
    }

    public get logLevel(): AdjustLogLevel {
        return this._logLevel;
    }

    public set logLevel(logLevel: AdjustLogLevel) {
        this._logLevel = logLevel;
    }

    public get eventBufferingEnabled(): boolean {
        return this._eventBufferingEnabled;
    }

    public set eventBufferingEnabled(value: boolean) {
        this._eventBufferingEnabled = value;
    }
}