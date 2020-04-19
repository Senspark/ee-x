package com.ee.admob;

import android.app.Activity;
import android.content.Context;

import androidx.annotation.NonNull;

import com.ee.ads.MessageHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
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

    AdMobRewardedAd(@NonNull Activity activity, @NonNull Context context, @NonNull String adId) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _messageHelper = new MessageHelper("AdMobRewardedAd", adId);
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        _logger.info("destroy: id = " + _adId);
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _messageHelper = null;
    }

    private void registerHandlers() {
        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, _messageHelper.createInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, _messageHelper.destroyInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isLoaded());
            }
        }, _messageHelper.isLoaded());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                load();
                return "";
            }
        }, _messageHelper.load());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                show();
                return "";
            }
        }, _messageHelper.show());
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(_messageHelper.createInternalAd());
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd());
        _bridge.deregisterHandler(_messageHelper.isLoaded());
        _bridge.deregisterHandler(_messageHelper.load());
        _bridge.deregisterHandler(_messageHelper.show());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _ad = new RewardedAd(_context, _adId);
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _ad = null;
        return true;
    }

    private boolean isLoaded() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        return _ad.isLoaded();
    }

    private void load() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        RewardedAdLoadCallback adLoadCallback = new RewardedAdLoadCallback() {
            @Override
            public void onRewardedAdLoaded() {
                // Ad successfully loaded.
                Utils.checkMainThread();
                _bridge.callCpp(_messageHelper.onLoaded());
            }

            @Override
            public void onRewardedAdFailedToLoad(int errorCode) {
                // Ad failed to load.
                Utils.checkMainThread();
                _bridge.callCpp(_messageHelper.onFailedToLoad(), String.valueOf(errorCode));
            }
        };
        _ad.loadAd(builder.build(), adLoadCallback);
    }

    private void show() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        _ad.show(_activity, this);
    }

    @Override
    public void onRewardedAdOpened() {
        Utils.checkMainThread();
        _logger.info("onRewardedAdOpened");
    }

    @Override
    public void onRewardedAdClosed() {
        _logger.info("onAdClosed");
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onClosed(), Utils.toString(_rewarded));
    }

    @Override
    public void onUserEarnedReward(@NonNull RewardItem reward) {
        _logger.info("onUserEarnedReward");
        Utils.checkMainThread();
        _rewarded = true;
    }

    @Override
    public void onRewardedAdFailedToShow(int errorCode) {
        _logger.info("onRewardedAdFailedToShow");
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onFailedToShow(), String.valueOf(errorCode));
    }
}
