import { AdObserver } from "../../ads/IAd";
import { FullScreenAdResult, IFullScreenAd } from "../../ads/IFullScreenAd";
import { AsyncHelper } from "../../ads/internal/AsyncHelper";
import { IAsyncHelper } from "../../ads/internal/IAsyncHelper";
import { MessageHelper } from "../../ads/internal/MessageHelper";
import { IMessageBridge } from "../../core/IMessageBridge";
import { ObserverManager } from "../../core/ObserverManager";
import { Utils } from "../../core/Utils";
import { AdMob } from "./AdMob";

export class AdMobRewardedAd extends ObserverManager<AdObserver> implements IFullScreenAd {
    private readonly _bridge: IMessageBridge;
    private readonly _displayer: IAsyncHelper<FullScreenAdResult>;
    private readonly _plugin: AdMob;
    private readonly _adId: string;
    private readonly _messageHelper: MessageHelper;
    private readonly _loader: IAsyncHelper<boolean>;

    public constructor(
        bridge: IMessageBridge, displayer: IAsyncHelper<FullScreenAdResult>, plugin: AdMob, adId: string) {
        super();
        this._bridge = bridge;
        this._displayer = displayer;
        this._plugin = plugin;
        this._adId = adId;
        this._messageHelper = new MessageHelper("AdMobRewardedAd", adId);
        this._loader = new AsyncHelper<boolean>();

        this._bridge.registerHandler(_ => this.onLoaded(), this._messageHelper.onLoaded);
        this._bridge.registerHandler(this.onFailedToLoad, this._messageHelper.onFailedToLoad);
        this._bridge.registerHandler(this.onFailedToShow, this._messageHelper.onFailedToShow);
        this._bridge.registerHandler(message => this.onClosed(Utils.toBool(message)), this._messageHelper.onClosed);
    }

    public void Destroy() {
        _bridge.DeregisterHandler(_messageHelper.OnLoaded);
        _bridge.DeregisterHandler(_messageHelper.OnFailedToLoad);
        _bridge.DeregisterHandler(_messageHelper.OnFailedToShow);
        _bridge.DeregisterHandler(_messageHelper.OnClosed);
        _plugin.DestroyRewardedAd(_adId);
    }

    public bool IsLoaded {
    get {
        var response = _bridge.Call(_messageHelper.IsLoaded);
        return Utils.ToBool(response);
    }
}

    public Task < bool > Load() {
    return _loader.Process(
        () => _bridge.Call(_messageHelper.Load),
        result => {
            // OK.
        });
}

    public Task < FullScreenAdResult > Show() {
    return _displayer.Process(
        () => _bridge.Call(_messageHelper.Show),
        result => {
            // OK.
        });
}

    private void OnLoaded() {
    if (_loader.IsProcessing) {
        _loader.Resolve(true);
    } else {
        Assert.IsTrue(false);
    }
    DispatchEvent(observer => observer.OnLoaded?.Invoke());
}

    private void OnFailedToLoad(string message) {
    if (_loader.IsProcessing) {
        _loader.Resolve(false);
    } else {
        Assert.IsTrue(false);
    }
}

    private void OnFailedToShow(string message) {
    if (_displayer.IsProcessing) {
        _displayer.Resolve(FullScreenAdResult.Failed);
    } else {
        Assert.IsTrue(false);
    }
}

    private void OnClosed(bool rewarded) {
    if (_displayer.IsProcessing) {
        _displayer.Resolve(rewarded
            ? FullScreenAdResult.Completed
            : FullScreenAdResult.Canceled);
    } else {
        Assert.IsTrue(false);
    }
}
}