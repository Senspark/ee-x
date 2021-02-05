import * as ee from '@senspark/ee-x';
import { BannerAdScene } from './BannerAdScene';
import { FullScreenAdScene } from './FullScreenAdScene';
import { ServiceUtils } from './ServiceUtils';

const { ccclass, property } = cc._decorator;

@ccclass
export class MainScene extends cc.Component {
    private _initialized = false;
    private _adsManager?: ee.IAdsManager;

    protected onLoad(): void {
        ServiceUtils.initialize();
        const fileName = cc.sys.os === cc.sys.OS_ANDROID
            ? `config/ads_config_android`
            : `config/ads_config_ios`;
        cc.resources.load<cc.JsonAsset>(fileName, (err, res) => {
            const adsManager = new ee.DefaultAdsManager(JSON.stringify(res.json));
            ee.Utils.noAwait(async () => {
                await adsManager.initialize();
            });
            this._adsManager = adsManager;
            this._initialized = true;
        });
    }

    public onTestBannerAdButtonPressed(): void {
        if (!this._initialized) {
            return;
        }
        const adsManager = this._adsManager;
        ee.Utils.noAwait(async () => {
            await new Promise(resolve => cc.director.loadScene(`BannerAdScene`, () => resolve(null)));
            const scene = cc.Canvas.instance.getComponentInChildren(BannerAdScene);
            scene.adsManager = adsManager;
            scene.execute();
        });
    }

    public onTestFullScreenAdButtonPressed(): void {
        if (!this._initialized) {
            return;
        }
        const adsManager = this._adsManager;
        ee.Utils.noAwait(async () => {
            await new Promise(resolve => cc.director.loadScene(`FullScreenAdScene`, () => resolve(null)));
            const scene = cc.Canvas.instance.getComponentInChildren(FullScreenAdScene);
            scene.adsManager = adsManager;
        });
    }
}
