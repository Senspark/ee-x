import {AdFormat, AdNetwork, AdRevenue, IapRevenue, ILogger, IMessageBridge, Utils,} from "../../core";
import {IFirebaseAnalytics} from "../IFirebaseAnalytics";

type Destroyer = () => void;

export class FirebaseAnalytics implements IFirebaseAnalytics {
    private readonly kTag = `FirebaseAnalytics`;
    private readonly kPrefix = "FirebaseAnalyticsBridge";
    private readonly kInitialize = `${this.kPrefix}Initialize`;
    private readonly kSetUserProperty = `${this.kPrefix}SetUserProperty`;
    private readonly kTrackScreen = `${this.kPrefix}TrackScreen`;
    private readonly kLogEvent = `${this.kPrefix}LogEvent`;

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

    public async initialize(appIdentity: string): Promise<boolean> {
        this._appIdentity = appIdentity;
        const response = await this._bridge.callAsync(this.kInitialize);
        return Utils.toBool(response);
    }

    public setUserProperty(key: string, value: string): void {
        const request = {
            key,
            value,
        };
        this._bridge.call(this.kSetUserProperty, JSON.stringify(request));
    }

    public trackScreen(name: string): void {
        this._bridge.call(this.kTrackScreen, name);
    }

    public logEvent(name: string, parameters: { [key: string]: string | number; }): void {
        const request = {
            name,
            parameters,
        };
        this._bridge.call(this.kLogEvent, JSON.stringify(request));
    }

    public logAdRevenue(adRevenue: AdRevenue): void {
        if (adRevenue.mediationNetwork === AdNetwork.AdMob) {
            // https://firebase.google.com/docs/analytics/measure-ad-revenue#implementation-admob
            // Logged automatically.
            return;
        }

        const evName: string = adRevenue.isValid() ? "ad_impression" : "ad_impression_error";

        let adFormat: string;
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

        let adPlatform: string;
        switch (adRevenue.mediationNetwork) {
            case AdNetwork.AppLovinMax:
                adPlatform = "AppLovin";
                break;
            // case AdNetwork.IronSource:
            //     adPlatform = "IronSource";
            //     break;
            default:
                adPlatform = "Others";
                break;
        }

        const request = {
            name: evName,
            parameters: {
                ad_platform: adPlatform,
                ad_source: adRevenue.monetizationNetwork,
                ad_unit: adRevenue.adUnit,
                ad_format: adFormat,
                value: adRevenue.revenue,
                currency: adRevenue.currencyCode,
            }
        };

        this._bridge.call(this.kLogEvent, JSON.stringify(request));
    }

    public logIapRevenue(iapRevenue: IapRevenue): void {
        // Prepare name
        let pIdShorten: string = iapRevenue.productId;
        if (this._appIdentity) {
            pIdShorten = pIdShorten.replace(this._appIdentity + ".", "")
        }

        pIdShorten = pIdShorten.replace("com.senspark.", "")
        pIdShorten = pIdShorten.replace(/\./g, '_').replace(/-/g, '_');

        this.logEvent("conv_buy_iap_" + pIdShorten, {});

        this.logEvent("conv_buy_iap", {
            package_name: iapRevenue.productId,
            order_id: iapRevenue.orderId,
        });
    }

    public pushGameLevel(levelNo: string, levelMode: string): void {
        // do nothing
    }

    public popGameLevel(winGame: boolean): void {
        // do nothing
    }
}