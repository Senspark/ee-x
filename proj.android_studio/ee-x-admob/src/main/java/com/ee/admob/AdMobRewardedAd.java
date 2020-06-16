package com.ee.admob;

import android.app.Activity;
import android.content.Context;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.MessageHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.internal.Thread;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.rewarded.RewardItem;
import com.google.android.gms.ads.rewarded.RewardedAd;
import com.google.android.gms.ads.rewarded.RewardedAdCallback;
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by KietLe on 12/03/19.
 */

class AdMobRewardedAd extends RewardedAdCallback {
    private static final Logger _logger = new Logger(AdMobRewardedAd.class.getName());

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private String _adId;
    private MessageHelper _messageHelper;
    private boolean _rewarded;
    private RewardedAd _ad;

    AdMobRewardedAd(@NonNull Context context,
                    @Nullable Activity activity,
                    @NonNull IMessageBridge bridge,
                    @NonNull String adId) {
        _logger.info("constructor: adId = %s", adId);
        Thread.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = bridge;
        _adId = adId;
        _messageHelper = new MessageHelper("AdMobRewardedAd", adId);
        createInternalAd();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
    }

    void onDestroy(@NonNull Activity activity) {
        assertThat(_activity).isEqualTo(activity);
        _activity = null;
    }

    void destroy() {
        _logger.info("destroy: adId = %s", _adId);
        Thread.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _messageHelper = null;
    }

    private void registerHandlers() {
        _bridge.registerHandler(_messageHelper.createInternalAd(), message ->
            Utils.toString(createInternalAd()));

        _bridge.registerHandler(_messageHelper.destroyInternalAd(), message ->
            Utils.toString(destroyInternalAd()));

        _bridge.registerHandler(_messageHelper.isLoaded(), message ->
            Utils.toString(isLoaded()));

        _bridge.registerHandler(_messageHelper.load(), message -> {
            load();
            return "";
        });

        _bridge.registerHandler(_messageHelper.show(), message -> {
            show();
            return "";
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(_messageHelper.createInternalAd());
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd());
        _bridge.deregisterHandler(_messageHelper.isLoaded());
        _bridge.deregisterHandler(_messageHelper.load());
        _bridge.deregisterHandler(_messageHelper.show());
    }

    private boolean createInternalAd() {
        Thread.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _ad = new RewardedAd(_context, _adId);
        return true;
    }

    private boolean destroyInternalAd() {
        Thread.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _ad = null;
        return true;
    }

    private boolean isLoaded() {
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        return _ad.isLoaded();
    }

    private void load() {
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        RewardedAdLoadCallback adLoadCallback = new RewardedAdLoadCallback() {
            @Override
            public void onRewardedAdLoaded() {
                // Ad successfully loaded.
                Thread.checkMainThread();
                _bridge.callCpp(_messageHelper.onLoaded());
            }

            @Override
            public void onRewardedAdFailedToLoad(int errorCode) {
                // Ad failed to load.
                Thread.checkMainThread();
                _bridge.callCpp(_messageHelper.onFailedToLoad(), String.valueOf(errorCode));
            }
        };
        _ad.loadAd(builder.build(), adLoadCallback);
    }

    private void show() {
        _logger.info("show");
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        _ad.show(_activity, this);
    }

    @Override
    public void onRewardedAdOpened() {
        Thread.checkMainThread();
        _logger.info("onRewardedAdOpened");
    }

    @Override
    public void onRewardedAdClosed() {
        _logger.info("onAdClosed");
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onClosed(), Utils.toString(_rewarded));
    }

    @Override
    public void onUserEarnedReward(@NonNull RewardItem reward) {
        _logger.info("onUserEarnedReward");
        Thread.checkMainThread();
        _rewarded = true;
    }

    @Override
    public void onRewardedAdFailedToShow(int errorCode) {
        _logger.info("onRewardedAdFailedToShow");
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onFailedToShow(), String.valueOf(errorCode));
    }
}
