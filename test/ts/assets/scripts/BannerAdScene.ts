import * as ee from '@senspark/ee-x';

const { ccclass, property } = cc._decorator;

@ccclass
export class BannerAdScene extends cc.Component {
    private _adsManager?: ee.IAdsManager;

    public get adsManager(): ee.IAdsManager {
        return this._adsManager;
    }

    public set adsManager(value: ee.IAdsManager | undefined) {
        this._adsManager = value;
    }

    public execute(): void {
        // FIXME.
    }
}
