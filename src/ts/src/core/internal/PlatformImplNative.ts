import {IMessageBridge} from "../IMessageBridge";
import {Utils} from "../Utils";
import {IPlatformImpl} from "./IPlatformImpl";
import {SafeInset} from "../SafeInset";

export class PlatformImplNative implements IPlatformImpl {
    private readonly kPrefix = "Platform_";
    private readonly kIsApplicationInstalled = this.kPrefix + "isApplicationInstalled";
    private readonly kOpenApplication = this.kPrefix + "openApplication";
    private readonly kGetApplicationId = this.kPrefix + "getApplicationId";
    private readonly kGetApplicationName = this.kPrefix + "getApplicationName";
    private readonly kGetVersionName = this.kPrefix + "getVersionName";
    private readonly kGetVersionCode = this.kPrefix + "getVersionCode";
    private readonly kGetApplicationSignatures = this.kPrefix + "getApplicationSignatures";
    private readonly kIsInstantApp = this.kPrefix + "isInstantApp";
    private readonly kIsTablet = this.kPrefix + "isTablet";
    private readonly kGetDensity = this.kPrefix + "getDensity";
    private readonly kGetViewSize = this.kPrefix + "getViewSize";
    private readonly kGetScreenSize = this.kPrefix + "getScreenSize";
    private readonly kGetDeviceId = this.kPrefix + "getDeviceId";
    private readonly kGetSafeInset = this.kPrefix + "getSafeInset";
    private readonly kSendMail = this.kPrefix + "sendMail";
    private readonly kTestConnection = this.kPrefix + "testConnection";
    private readonly kShowInstallPrompt = this.kPrefix + "showInstallPrompt";
    private readonly kGetInstallReferrer = this.kPrefix + "getInstallReferrer";

    private readonly _bridge: IMessageBridge;

    public constructor(bridge: IMessageBridge) {
        this._bridge = bridge;
    }

    public isApplicationInstalled(applicationId: string): boolean {
        const repsonse = this._bridge.call(this.kIsApplicationInstalled, applicationId);
        return Utils.toBool(repsonse);
    }

    public openApplication(applicationId: string): boolean {
        const response = this._bridge.call(this.kOpenApplication, applicationId);
        return Utils.toBool(response);
    }

    public getApplicationId(): string {
        return this._bridge.call(this.kGetApplicationId);
    }

    public getApplicationName(): string {
        return this._bridge.call(this.kGetApplicationName);
    }

    public getVersionName(): string {
        return this._bridge.call(this.kGetVersionName);
    }

    public getVersionCode(): string {
        return this._bridge.call(this.kGetVersionCode);
    }

    public getSha1Signature(): string {
        throw new Error("Method not implemented.");
    }

    public isInstantApp(): boolean {
        throw new Error("Method not implemented.");
    }

    public isTablet(): boolean {
        const response = this._bridge.call(this.kIsTablet);
        return Utils.toBool(response);
    }

    public getDensity(): number {
        const response = this._bridge.call(this.kGetDensity);
        return parseFloat(response);
    }

    public getViewSize(): [number, number] {
        const response = this._bridge.call(this.kGetViewSize);
        const json: {
            width: number,
            height: number,
        } = JSON.parse(response);
        return [json.width, json.height];
    }

    public getScreenSize(): [number, number] {
        const response = this._bridge.call(this.kGetScreenSize);
        const json: {
            width: number,
            height: number,
        } = JSON.parse(response);
        return [json.width, json.height];
    }

    public async getDeviceId(): Promise<string> {
        const response = await this._bridge.callAsync(this.kGetDeviceId);
        return response;
    }

    public sendMail(recipient: string, subject: string, body: string): boolean {
        const request = {
            [`recipient`]: recipient,
            [`subject`]: subject,
            [`body`]: body,
        };
        const response = this._bridge.call(this.kSendMail, JSON.stringify(request));
        return Utils.toBool(response);
    }

    public async testConnection(hostName: string, timeOut: number): Promise<boolean> {
        const request = {
            [`host_name`]: hostName,
            [`time_out`]: timeOut,
        };
        const response = await this._bridge.callAsync(this.kTestConnection, JSON.stringify(request));
        return Utils.toBool(response);
    }

    public getSafeInset(): SafeInset {
        const response = this._bridge.call(this.kGetSafeInset);
        const safeInset = JSON.parse(response);
        return safeInset;
    }
}