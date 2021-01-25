export interface IPlatformImpl {
    isApplicationInstalled(applicationId: string): boolean;
    openApplication(applicationId: string): boolean;
    getApplicationId(): string;
    getApplicationName(): string;
    getVersionName(): string;
    getVersionCode(): string;
    getSha1Signature(): string;
    isInstantApp(): boolean;
    isTablet(): boolean;
    getDensity(): number;
    getViewSize(): [number, number];
    getScreenSize(): [number, number];
    getDeviceId(): Promise<string>;
    sendMail(recipient: string, subject: string, body: string): boolean;
    testConnection(hostName: string, timeOut: number): Promise<boolean>;
}