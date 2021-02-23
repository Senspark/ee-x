import * as ee from '@senspark/ee-x';

const { ccclass, property } = cc._decorator;

@ccclass
export class FullScreenAdScene extends cc.Component {
    @property({ type: cc.Label, visible: true })
    private readonly _resultText: cc.Label | null = null;

    private _adsManager?: ee.IAdsManager;

    public get adsManager(): ee.IAdsManager {
        return this._adsManager;
    }

    public set adsManager(value: ee.IAdsManager | undefined) {
        this._adsManager = value;
    }

    private updateResult(result: ee.AdResult): void {
        switch (result) {
            case ee.AdResult.NotInitialized:
                this._resultText.string = "Not Initialized";
                break;
            case ee.AdResult.NotConfigured:
                this._resultText.string = "Not Configured";
                break;
            case ee.AdResult.Capped:
                this._resultText.string = "Capped";
                break;
            case ee.AdResult.NoInternet:
                this._resultText.string = "No Internet";
                break;
            case ee.AdResult.NotLoaded:
                this._resultText.string = "Not Loaded";
                break;
            case ee.AdResult.Failed:
                this._resultText.string = "Failed";
                break;
            case ee.AdResult.Canceled:
                this._resultText.string = "Canceled";
                break;
            case ee.AdResult.Completed:
                this._resultText.string = "Completed";
                break;
        }
    }

    public onShowInterstitialAdButtonPressed(): void {
        ee.Utils.noAwait(async () => {
            this._resultText.string = `---`;
            const result = await this.adsManager.interstitialAd.show();
            this.updateResult(result);
        });
    }

    public onShowRewardedAdButtonPressed(): void {
        ee.Utils.noAwait(async () => {
            this._resultText.string = `---`;
            const result = await this.adsManager.rewardedAd.show();
            this.updateResult(result);
        });
    }
}
