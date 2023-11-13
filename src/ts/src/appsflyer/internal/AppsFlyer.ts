import {AdFormat, AdNetwork, AdRevenue, IapRevenue, ILogger, IMessageBridge, Utils,} from "../../core";
import {IAppsFlyer} from "../IAppsFlyer";

type Destroyer = () => void;

export class AppsFlyer implements IAppsFlyer {
    private readonly kTag = `AppsFlyer`;
    private readonly kPrefix = "AppsFlyerBridge";
    private readonly kInitialize = this.kPrefix + "Initialize";
    private readonly kStartTracking = this.kPrefix + "StartTracking";
    private readonly kGetDeviceId = this.kPrefix + "GetDeviceId";
    private readonly kSetDebugEnabled = this.kPrefix + "SetDebugEnabled";
    private readonly kSetStopTracking = this.kPrefix + "SetStopTracking";
    private readonly kTrackEvent = this.kPrefix + "TrackEvent";
    private readonly kLogAdRevenue = this.kPrefix + "LogAdRevenue";

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

    private _appIdentity: string = ``;

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

    public initialize(devKey: string, iosAppId: string, appIdentity: string): void {
        const request = {
            devKey: devKey,
            iosAppId: iosAppId
        };
        this._appIdentity = appIdentity;
        this._bridge.call(this.kInitialize, JSON.stringify(request));
    }

    public startTracking() {
        this._bridge.call(this.kStartTracking);
    }

    public getDeviceId(): string {
        return this._bridge.call(this.kGetDeviceId);
    }

    public setDebugEnabled(enabled: boolean): void {
        this._bridge.call(this.kSetDebugEnabled, Utils.toString(enabled));
    }

    public setStopTracking(enabled: boolean): void {
        this._bridge.call(this.kSetStopTracking, Utils.toString(enabled));
    }

    public trackEvent(name: string, parameters: { [key: string]: number | string }): void {
        const request = {
            name: name,
            values: parameters,
        };
        this._bridge.call(this.kTrackEvent, JSON.stringify(request));
    }

    public logAdRevenue(adRevenue: AdRevenue) {
        if (!adRevenue.isValid()) {
            return;
        }

        let mediationNetwork;
        switch (adRevenue.mediationNetwork) {
            case AdNetwork.AdMob:
                mediationNetwork = "admob";
                break;
            case AdNetwork.AppLovinMax:
                mediationNetwork = "applovin";
                break;
            default:
                mediationNetwork = "others";
                break;
        }

        let adFormat;
        switch (adRevenue.adFormat) {
            case AdFormat.AppOpen:
                adFormat = "App Open";
                break;
            case AdFormat.Banner:
                adFormat = "Banner";
                break;
            case AdFormat.Interstitial:
                adFormat = "Interstitial";
                break;
            case AdFormat.Rectangle:
                adFormat = "Rect";
                break;
            case AdFormat.Rewarded:
                adFormat = "Rewarded";
                break;
            case AdFormat.RewardedInterstitial:
                adFormat = "Rewarded Interstitial";
                break;
            default:
                adFormat = "Others";
                break;
        }

        const request = {
            mediationNetwork: mediationNetwork,
            monetizationNetwork: adRevenue.monetizationNetwork,
            revenue: adRevenue.revenue,
            currencyCode: adRevenue.currencyCode,
            adFormat: adFormat,
            adUnit: adRevenue.adUnit,
        }

        const output = JSON.stringify(request);
        this._logger.info(`${this.kTag}: logAdRevenue: ${output}`);
        this._bridge.call(this.kLogAdRevenue, output);
    }

    public logIapRevenue(iapRevenue: IapRevenue): void {
        let pIdShorten: string = iapRevenue.productId;

        if (this._appIdentity !== '') {
            pIdShorten = pIdShorten.replace(`${this._appIdentity}.`, ``);
        }

        pIdShorten = pIdShorten.replace(`com.senspark.`, ``);
        pIdShorten = pIdShorten.replace(/\./g, '_');
        pIdShorten = pIdShorten.replace(/-/g, '_');

        const evName: string = "conv_buy_iap_" + pIdShorten;

        const json = {
            name: evName,
            values: {
                af_revenue: 0,
                af_currency: iapRevenue.currencyCode,
                af_quantity: 1,
                af_content_type: "iap_product",
                af_content_id: iapRevenue.productId,
            }
        };

        const output: string = JSON.stringify(json);
        this._logger.info(`${this.kPrefix} ${evName}: ${output}`);
        this._bridge.call(this.kTrackEvent, output);
    }

    public pushGameLevel(levelNo: number, levelMode: string): void {
        // do nothing
    }

    public popGameLevel(winGame: boolean): void {
        // do nothing
    }
}