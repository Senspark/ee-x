package com.ee.admob;

import android.app.Activity;
import android.content.Context;

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

import androidx.annotation.NonNull;

/**
 * Created by KietLe on 12/03/19.
 */

class AdMobRewardVideoAd extends RewardedAdCallback {
    private static final Logger _logger = new Logger(AdMobRewardVideoAd.class.getName());

    private Context        _context;
    private Activity       _activity;
    private RewardedAd     _rewardedVideoAd;
    private String         _adId;
    private IMessageBridge _bridge;

    private String kCreateInternalAd() {
        return "AdMobRewardVideoAd_createInternalAd_" + _adId;
    }

    private String kDestroyInternalAd() {
        return "AdMobRewardVideoAd_destroyInternalAd_" + _adId;
    }

    private String kIsLoaded() {
        return "AdMobRewardVideoAd_isLoaded_" + _adId;
    }

    private String kLoad() {
        return "AdMobRewardVideoAd_load_" + _adId;
    }

    private String kShow() {
        return "AdMobRewardVideoAd_show_" + _adId;
    }

    private String kOnLoaded() {
        return "AdMobRewardVideoAd_onLoaded_" + _adId;
    }

    private String kOnFailedToLoad() {
        return "AdMobRewardVideoAd_onFailedToLoad_" + _adId;
    }

    private String kOnReward() {
        return "AdMobRewardVideoAd_onReward_" + _adId;
    }

    private String kOnFailedToShow() {
        return "AdMobRewardVideoAd_onFailedToShow_" + _adId;
    }

    private String kOnClosed() {
        return "AdMobRewardVideoAd_onClosed_" + _adId;
    }


    public AdMobRewardVideoAd(@NonNull Activity activity, @NonNull Context context, @NonNull String placementId) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _adId = placementId;
        _bridge = MessageBridge.getInstance();

        registerHandlers();
    }

    void destroy() {
        _logger.info("destroy: id = " + _adId);
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _adId = null;
        _bridge = null;
    }

    private void registerHandlers() {
        Utils.checkMainThread();

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, kCreateInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, kDestroyInternalAd());


        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isLoaded());
            }
        }, kIsLoaded());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(loadRewardedVideo());
            }
        }, kLoad());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(showRewardVideo());
            }
        }, kShow());
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();

        _bridge.deregisterHandler(kCreateInternalAd());
        _bridge.deregisterHandler(kDestroyInternalAd());
        _bridge.deregisterHandler(kIsLoaded());
        _bridge.deregisterHandler(kLoad());
        _bridge.deregisterHandler(kShow());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_rewardedVideoAd != null) {
            return false;
        }
        _rewardedVideoAd = new RewardedAd(_context, _adId);
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_rewardedVideoAd == null) {
            return false;
        }
        _rewardedVideoAd = null;
        return true;
    }

    private boolean isLoaded() {
        Utils.checkMainThread();
        if (_rewardedVideoAd == null) {
            return false;
        }
        return _rewardedVideoAd.isLoaded();
    }

    private boolean loadRewardedVideo() {
        Utils.checkMainThread();
        if (_rewardedVideoAd == null) {
            return false;
        }
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();

        RewardedAdLoadCallback adLoadCallback = new RewardedAdLoadCallback() {
            @Override
            public void onRewardedAdLoaded() {
                // Ad successfully loaded.
                Utils.checkMainThread();
                _bridge.callCpp(kOnLoaded());
            }

            @Override
            public void onRewardedAdFailedToLoad(int errorCode) {
                // Ad failed to load.
                Utils.checkMainThread();
                _bridge.callCpp(kOnFailedToLoad(), String.valueOf(errorCode));
            }
        };

        _rewardedVideoAd.loadAd(builder.build(), adLoadCallback);

        return true;
    }

    private boolean showRewardVideo() {
        Utils.checkMainThread();
        if (_rewardedVideoAd == null) {
            return false;
        }

        _rewardedVideoAd.show(_activity, this);
        return true;
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
        _bridge.callCpp(kOnClosed());
    }

    @Override
    public void onUserEarnedReward(@NonNull RewardItem reward) {
        _logger.info("onUserEarnedReward");
        Utils.checkMainThread();
        _bridge.callCpp(kOnReward());
    }

    @Override
    public void onRewardedAdFailedToShow(int errorCode) {
        _logger.info("onRewardedAdFailedToShow");
        Utils.checkMainThread();
        _bridge.callCpp(kOnFailedToShow());
    }
}
