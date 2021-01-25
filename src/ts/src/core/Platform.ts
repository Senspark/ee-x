import { IMessageBridge } from "./IMessageBridge";
import {
    IPlatformImpl,
    PlatformImplNative,
} from "./internal";

export class Platform {
    private static _impl: IPlatformImpl;

    public static initialize(bridge: IMessageBridge) {
        this._impl = new PlatformImplNative(bridge);
    }

    public static isApplicationInstalled(applicationId: string): boolean {
        return this._impl.isApplicationInstalled(applicationId);
    }

    public static openApplication(applicationId: string): boolean {
        return this._impl.openApplication(applicationId);
    }

    public static getApplicationId(): string {
        return this._impl.getApplicationId();
    }

    public static getApplicationName(): string {
        return this._impl.getApplicationName();
    }

    public static getVersionName(): string {
        return this._impl.getVersionName();
    }

    public static getVersionCode(): string {
        return this._impl.getVersionCode();
    }

    public static getSha1Signature(): string {
        return this._impl.getSha1Signature();

    }

    public static isInstantApp(): boolean {
        return this._impl.isInstantApp();
    }

    public static isTablet(): boolean {
        return this._impl.isTablet();
    }

    public static getDensity(): number {
        return this._impl.getDensity();
    }

    public static getViewSize(): [number, number] {
        return this._impl.getViewSize();
    }

    public static getScreenSize(): [number, number] {
        return this._impl.getScreenSize();
    }

    public static getDeviceId(): Promise<string> {
        return this._impl.getDeviceId();
    }

    public static sendMail(recipient: string, subject: string, body: string): boolean {
        return this._impl.sendMail(recipient, subject, body);
    }

    public static testConnection(hostName: string, timeOut: number): Promise<boolean> {
        return this._impl.testConnection(hostName, timeOut);
    }
}