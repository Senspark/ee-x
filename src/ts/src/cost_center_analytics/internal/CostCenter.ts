import {AdFormat, AdNetwork, AdRevenue, IapRevenue, ILogger, IMessageBridge,} from "../../core";
import {ICostCenter} from "../ICostCenter";

type Destroyer = () => void;

export class CostCenter implements ICostCenter {
    // Dùng Firebase để Log event
    // Dùng AppsFlyer để listen event
    private readonly kTag = `CostCenter`;
    private readonly kPrefix = "CostCenterAnalyticsBridge";
    private readonly kFirebasePrefix = "FirebaseAnalyticsBridge";
    private readonly kAppsFlyerPrefix = "AppsFlyerBridge";

    private readonly kLogEvent = this.kFirebasePrefix + "LogEvent";
    private readonly kOnPurchaseValidated = this.kAppsFlyerPrefix + "OnPurchaseValidated";

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

    private _gameLevelData: GameLevelData | null = null;
    private _lastIapRevenue: IapRevenue | null = null;

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

    public initialize(): void {
        this._bridge.registerHandler((json: string) => {
            this.onPurchaseValidated(json);
        }, this.kOnPurchaseValidated);
    }

    public logAdRevenue(adRevenue: AdRevenue) {
        // Log ad revenue dùng Firebase (khác với ad_impression đã log bên Firebase)

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
                adFormat = "app_open";
                break;
            case AdFormat.Banner:
                adFormat = "banner";
                break;
            case AdFormat.Interstitial:
                adFormat = "inter";
                break;
            case AdFormat.Rectangle:
                adFormat = "banner_rect";
                break;
            case AdFormat.Rewarded:
                adFormat = "reward";
                break;
            case AdFormat.RewardedInterstitial:
                adFormat = "reward_inter";
                break;
            default:
                adFormat = "others";
                break;
        }

        const parameters = {
            ad_format: adFormat,
            value: adRevenue.revenue,
            currency: adRevenue.currencyCode,
        };
        this.addGameLevelDataToParams(parameters);

        const request = {
            name: "ad_revenue_sdk",
            parameters: parameters
        };

        const output = JSON.stringify(request);
        this._logger.info(`${this.kTag}: logAdRevenue: ${output}`);
        this._bridge.call(this.kLogEvent, output);
    }

    public logIapRevenue(iapRevenue: IapRevenue): void {
        this._lastIapRevenue = iapRevenue;
        this._logger.info(`${this.kTag}: Add to pending: ${JSON.stringify(iapRevenue)}`);
    }

    public pushGameLevel(levelNo: string, levelMode: string): void {
        this._gameLevelData = {
            levelNo: levelNo,
            levelMode: levelMode,
        };

        const parameters = {};
        this.addGameLevelDataToParams(parameters);

        const request = {
            name: "level_start",
            "parameters": parameters,
        }

        const output = JSON.stringify(request);
        this._logger.info(`${this.kTag}: pushGameLevel: ${output}`);
        this._bridge.call(this.kLogEvent, output);
    }

    public popGameLevel(winGame: boolean): void {
        if (!this._gameLevelData) {
            return;
        }

        const winStr = winGame ? "True" : "False";
        const parameters = {
            success: winStr,
        };
        this.addGameLevelDataToParams(parameters);

        const request = {
            name: "level_end",
            parameters: parameters,
        };

        const output = JSON.stringify(request);
        this._logger.info(`${this.kTag}: popGameLevel: ${output}`);
        this._bridge.call(this.kLogEvent, output);

        this._gameLevelData = null;
    }

    private addGameLevelDataToParams(parameters: any): void {
        if (!this._gameLevelData) {
            return;
        }
        parameters.level = this._gameLevelData.levelNo;
        parameters.level_mode = this._gameLevelData.levelMode;
    }

    private onPurchaseValidated(json: string): void {
        try {
            const data: any = JSON.parse(json);
            // const isSuccess: boolean = data["isSuccess"];
            const isTestPurchase: boolean = data["isTestPurchase"];
            const orderId: string = data["orderId"];
            // const productId: string = data["productId"];

            if (!this._lastIapRevenue) {
                this._logger.error(`${this.kTag}: last iap revenue is null`);
                return;
            }
            if (this._lastIapRevenue.orderId !== orderId) {
                this._logger.error(`${this.kTag}: orderId or productId doesn't match`);
                return;
            }
            this.logIapRevenueImpl(this._lastIapRevenue, isTestPurchase);
        } catch (e) {
            this._logger.error(`${this.kTag}: onPurchaseValidated: ${e}`)
        }
    }

    public logIapRevenueImpl(iapRevenue: IapRevenue, isTestPurchase: boolean): void {
        const parameters = {
            product_id: iapRevenue.productId,
            order_id: iapRevenue.orderId,
            value: iapRevenue.revenue,
            currency: iapRevenue.currencyCode,
        };
        this.addGameLevelDataToParams(parameters);

        const json = {
            name: isTestPurchase ? "iap_sdk_test" : "iap_sdk",
            values: parameters
        };

        const output: string = JSON.stringify(json);
        this._logger.info(`${this.kPrefix} ${output}`);
        this._bridge.call(this.kLogEvent, output);
    }
}

class GameLevelData {
    public levelNo: string = ``;
    public levelMode: string = ``;
}