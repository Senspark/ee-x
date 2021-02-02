import {
    ILogger,
    IMessageBridge,
    Utils,
} from "../../core";
import { FirebaseFetchStatus } from "../FirebaseFetchStatus";
import { IFirebaseRemoteConfig } from "../IFirebaseRemoteConfig";

type Destroyer = () => void;

export class FirebaseRemoteConfig implements IFirebaseRemoteConfig {
    private readonly kTag = "FirebaseRemoteConfigBridge";
    private readonly kPrefix = "FirebaseRemoteConfigBridge";
    private readonly kInitialize = `${this.kPrefix}Initialize`;
    private readonly kSetSettings = `${this.kPrefix}SetSettings`;
    private readonly kFetch = `${this.kPrefix}Fetch`;
    private readonly kActivate = `${this.kPrefix}Activate`;
    private readonly kSetDefaults = `${this.kPrefix}SetDefaults`;
    private readonly kGetBool = `${this.kPrefix}GetBool`;
    private readonly kGetLong = `${this.kPrefix}GetLong`;
    private readonly kGetDouble = `${this.kPrefix}GetDouble`;
    private readonly kGetString = `${this.kPrefix}GetString`;

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
        this._logger.debug(`${this.kTag}: constructor`);
    }

    public destroy(): void {
        this._logger.debug(`${this.kTag}: destroy`);
        this._destroyer();
    }

    public async initialize(): Promise<void> {
        await this._bridge.callAsync(this.kInitialize);
    }

    public async setSettings(fetchTimeOut: number, fetchInterval: number): Promise<void> {
        const request = {
            fetchTimeOut,
            fetchInterval,
        };
        await this._bridge.callAsync(this.kSetSettings, JSON.stringify(request));
    }

    public async fetch(fetchTimeOut: number): Promise<FirebaseFetchStatus> {
        const request = {
            fetchTimeOut,
        };
        const response = await this._bridge.callAsync(this.kFetch, JSON.stringify(request));
        switch (parseInt(response)) {
            case 0: return FirebaseFetchStatus.Success;
            case 1: return FirebaseFetchStatus.NoFetchYet;
            case 2: return FirebaseFetchStatus.Failure;
            case 3: return FirebaseFetchStatus.Throttled;
            default: return FirebaseFetchStatus.Failure;
        }
    }

    public async activate(): Promise<boolean> {
        const response = await this._bridge.callAsync(this.kActivate);
        return Utils.toBool(response);
    }

    public async setDefaults(defaults: { [key: string]: boolean | number | string }): Promise<void> {
        const request = {
            defaults,
        };
        await this._bridge.callAsync(this.kSetDefaults, JSON.stringify(request));
    }

    public async getBool(key: string): Promise<boolean> {
        const response = await this._bridge.callAsync(this.kGetBool, key);
        return Utils.toBool(response);
    }

    public async getLong(key: string): Promise<number> {
        const response = await this._bridge.callAsync(this.kGetLong, key);
        return parseInt(response);
    }

    public async getDouble(key: string): Promise<number> {
        const response = await this._bridge.callAsync(this.kGetDouble, key);
        return parseFloat(response);
    }

    public async getString(key: string): Promise<string> {
        const response = await this._bridge.callAsync(this.kGetString, key);
        return response;
    }
}