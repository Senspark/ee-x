package com.ee.facebook;

import android.content.Context;
import android.support.annotation.NonNull;

import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.InterstitialAdInterface;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;
import com.facebook.ads.RewardedVideoAd;
import com.facebook.ads.RewardedVideoAdListener;

/**
 * Created by Zinge on 10/11/17.
 */

class FacebookRewardVideoAd implements RewardedVideoAdListener {

    private String k__hasRewardedVideo() {
        return "FacebookAds_hasRewardedVideo_" + _placementId;
    }

    private String k__loadRewardedVideo() {
        return "FacebookAds_loadRewardedVideo_" + _placementId;
    }

    private String k__showRewardedVideo() {
        return "FacebookAds_showRewardedVideo_" + _placementId;
    }

    private String k__onRewarded() {
        return "FacebookAds_Video_onRewarded_" + _placementId;
    }

    private String k__onFailedToLoad() {
        return "FacebookAds_Video_onFailedToLoad_" + _placementId;
    }

    private String k__onLoaded() {
        return "FacebookAds_Video_onLoaded_" + _placementId;
    }

    private String k__onOpened() {
        return "FacebookAds_Video_onOpened_" + _placementId;
    }

    private String k__onClosed() {
        return "FacebookAds_Video_onClosed_" + _placementId;
    }

    private static final Logger _logger = new Logger(FacebookRewardVideoAd.class.getName());

    private Context _context;
    private RewardedVideoAd _rewardedVideoAd;
    private String _placementId;

    public FacebookRewardVideoAd(@NonNull Context context, @NonNull String placementId) {
        Utils.checkMainThread();
        _context = context;
        _placementId = placementId;

        _rewardedVideoAd = new RewardedVideoAd(_context, placementId);
        _rewardedVideoAd.setAdListener(this);

        registerHandlers();
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();

        _context = null;
        _placementId = null;
    }

    private void registerHandlers() {
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                loadRewardedVideo();
                return "";
            }
        }, k__loadRewardedVideo());


        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(hasRewardVideo());
            }
        }, k__hasRewardedVideo());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(showRewardVideo());
            }
        }, k__showRewardedVideo());
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__hasRewardedVideo());
        bridge.deregisterHandler(k__loadRewardedVideo());
        bridge.deregisterHandler(k__showRewardedVideo());
    }

    private boolean hasRewardVideo() {
        return _rewardedVideoAd.isAdLoaded();
    }

    private void loadRewardedVideo() {
        _rewardedVideoAd.loadAd();
    }

    private boolean showRewardVideo() {
        return _rewardedVideoAd.show();
    }

    @Override
    public void onRewardedVideoCompleted() {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onRewarded());
    }

    @Override
    public void onRewardedVideoClosed() {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onClosed());
    }

    @Override
    public void onAdClicked(Ad ad) {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onOpened());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onLoaded());
    }

    @Override
    public void onLoggingImpression(Ad ad) {

    }
}
