import { IPlugin } from "../core";
import { AdjustConfig } from "./AdjustConfig";

export interface IAdjust extends IPlugin {
    initialize(config: AdjustConfig): void;
    setEnabled(enabled: boolean): void;
    getAdvertisingIdentifier(): Promise<string>;
    getDeviceIdentifier(): string;
    setPushToken(token: string): void;
    trackEvent(token: string): void;
}