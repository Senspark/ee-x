export interface ILibraryAnalytics {
    logAdRevenue(adRevenue: AdRevenue): void;

    logIapRevenue(iapRevenue: IapRevenue): void;

    pushGameLevel(levelNo: number, levelMode: string): void;

    popGameLevel(winGame: boolean): void;
}

export enum AdNetwork {
    Null,
    AdMob,
    AppLovinMax
}

export enum AdFormat {
    Null,
    Banner,
    Rectangle,
    AppOpen,
    Interstitial,
    RewardedInterstitial,
    Rewarded,
}

export class AdRevenue {
    public mediationNetwork: AdNetwork = AdNetwork.Null;
    public monetizationNetwork: string = ``;
    public revenue: number = 0;
    public currencyCode: string = ``;
    public adFormat: AdFormat = AdFormat.Null;
    public adUnit: string = ``;

    public constructor(
        mediationNetwork: AdNetwork,
        monetizationNetwork: string,
        revenue: number,
        currencyCode: string,
        adFormat: AdFormat,
        adUnit: string
    ) {
        this.mediationNetwork = mediationNetwork;
        this.monetizationNetwork = monetizationNetwork;
        this.revenue = revenue;
        this.currencyCode = currencyCode;
        this.adFormat = adFormat;
        this.adUnit = adUnit;
    }

    public isValid() {
        return this.mediationNetwork !== AdNetwork.Null
            && this.monetizationNetwork !== ``
            && this.revenue > 0
            && this.currencyCode !== ``
            && this.adFormat !== AdFormat.Null
            && this.adUnit !== ``;
    }
}

export class IapRevenue {
    public revenue: number = 0;
    public currencyCode: string = ``;
    public productId: string = ``;
    public orderId: string = ``;
}