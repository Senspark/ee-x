package com.ee.ironsource;

import android.app.Activity;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.InterstitialListener;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class IronSource implements PluginProtocol, RewardedVideoListener, InterstitialListener {
    private static final String k__initialize        = "IronSource_initialize";
    private static final String k__hasRewardedVideo  = "IronSource_hasRewardedVideo";
    private static final String k__showRewardedVideo = "IronSource_showRewardedVideo";

    private static final String k__loadInterstitial = "IronSource_loadInterstitial";
    private static final String k__hasInterstitial  = "IronSource_hasInterstitial";
    private static final String k__showInterstitial = "IronSource_showInterstitial";

    private static final String k__onRewarded      = "IronSource_onRewarded";
    private static final String k__onFailed        = "IronSource_onFailed";
    private static final String k__onOpened        = "IronSource_onOpened";
    private static final String k__onClosed        = "IronSource_onClosed";
    private static final String k__onRewardClicked = "IronSource_onRewardClicked";

    private static final String k__onInterstitialFailed  = "IronSource_onInterstitialFailed";
    private static final String k__onInterstitialOpened  = "IronSource_onInterstitialOpened";
    private static final String k__onInterstitialClosed  = "IronSource_onInterstitialClosed";
    private static final String k__onInterstitialClicked = "IronSource_onInterstitialClicked";

    private static final Logger _logger = new Logger(IronSource.class.getName());

    private Activity _activity;
    private boolean  _initialized;
    private MessageBridge _bridge;

    public IronSource() {
        Utils.checkMainThread();
        _logger.debug("constructor begin.");
        _activity = null;
        _initialized = false;
        _bridge = MessageBridge.getInstance();
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "IronSource";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
        Utils.checkMainThread();
        com.ironsource.mediationsdk.IronSource.onResume(_activity);
    }

    @Override
    public void onPause() {
        Utils.checkMainThread();
        com.ironsource.mediationsdk.IronSource.onPause(_activity);
        Utils.checkMainThread();
    }

    @Override
    public void onDestroy() {
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        if (!_initialized) {
            return;
        }
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(null);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                assert _activity != null;
                String gameId = message;
                initialize(_activity, gameId);
                return "";
            }
        }, k__initialize);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(hasRewardedVideo());
            }
        }, k__hasRewardedVideo);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                showRewardedVideo(placementId);
                return "";
            }
        }, k__showRewardedVideo);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(hasInterstitial());
            }
        }, k__hasInterstitial);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                showInterstitial(placementId);
                return "";
            }
        }, k__showInterstitial);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                loadInterstitial();
                return "";
            }
        }, k__loadInterstitial);
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
        _bridge.deregisterHandler(k__hasRewardedVideo);
        _bridge.deregisterHandler(k__showRewardedVideo);

        _bridge.deregisterHandler(k__loadInterstitial);
        _bridge.deregisterHandler(k__hasInterstitial);
        _bridge.deregisterHandler(k__showInterstitial);
    }

    @Override
    public void onInterstitialAdReady() {
        _logger.debug("onInterstitialAdReady");
    }

    @Override
    public void onInterstitialAdLoadFailed(IronSourceError ironSourceError) {
        _logger.debug("onInterstitialAdLoadFailed");
    }

    @Override
    public void onInterstitialAdOpened() {
        _logger.debug("onInterstitialAdOpened");
        _bridge.callCpp(k__onInterstitialOpened);
    }

    @Override
    public void onInterstitialAdClosed() {
        _logger.debug("onInterstitialAdClosed");
        _bridge.callCpp(k__onInterstitialClosed);
    }

    @Override
    public void onInterstitialAdShowSucceeded() {
        _logger.debug("onInterstitialAdShowSucceeded");
    }

    @Override
    public void onInterstitialAdShowFailed(IronSourceError ironSourceError) {
        _logger.debug("onInterstitialAdShowFailed");
        _bridge.callCpp(k__onInterstitialFailed);
    }

    @Override
    public void onInterstitialAdClicked() {
        _logger.debug("onInterstitialAdClicked");
        _bridge.callCpp(k__onInterstitialClicked);
    }

    @Override
    public void onRewardedVideoAvailabilityChanged(boolean available) {
        _logger.info("onRewardedVideoAvailabilityChanged: " + available);
    }

    @Override
    public void onRewardedVideoAdRewarded(Placement placement) {
        _logger.debug("onRewardedVideoAdRewarded: " + placement.getPlacementName());
        _bridge.callCpp(k__onRewarded, placement.getPlacementName());
    }

    @Override
    public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {
        _logger.debug("onRewardedVideoAdShowFailed: " + ironSourceError.getErrorMessage());

        _bridge.callCpp(k__onFailed);
    }

    @Override
    public void onRewardedVideoAdClicked(Placement placement) {
        _logger.debug("onRewardedVideoAdClicked");

        _bridge.callCpp(k__onRewardClicked);
    }

    @Override
    public void onRewardedVideoAdOpened() {
        _logger.debug("onRewardedVideoAdOpened");
        _bridge.callCpp(k__onOpened);
    }

    @Override
    public void onRewardedVideoAdClosed() {
        _logger.debug("onRewardedVideoAdClosed");

        _bridge.callCpp(k__onClosed);
    }

    @Override
    public void onRewardedVideoAdStarted() {
        _logger.debug("onRewardedVideoAdStarted");
    }

    @Override
    public void onRewardedVideoAdEnded() {
        _logger.debug("onRewardedVideoAdEnded");
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull Activity activity, @NonNull String gameId) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }

        com.ironsource.mediationsdk.IronSource.setInterstitialListener(this);

        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(this);

        com.ironsource.mediationsdk.IronSource.init(activity, gameId, com.ironsource.mediationsdk.IronSource.AD_UNIT.REWARDED_VIDEO, com.ironsource.mediationsdk.IronSource.AD_UNIT.INTERSTITIAL);

        _initialized = true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedVideo() {
        Utils.checkMainThread();
        boolean value = com.ironsource.mediationsdk.IronSource.isRewardedVideoAvailable();
        _logger.debug("hasRewardedVideo " + value);
        return value;
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo(@NonNull String placementId) {
        _logger.debug("showRewardedVideo");
        com.ironsource.mediationsdk.IronSource.showRewardedVideo(placementId);
    }

    @SuppressWarnings("WeakerAccess")
    public void loadInterstitial() {
        _logger.debug("loadInterstitial");
        com.ironsource.mediationsdk.IronSource.loadInterstitial();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasInterstitial() {
        boolean value = com.ironsource.mediationsdk.IronSource.isInterstitialReady();
        _logger.debug("hasInterstitial " + value);
        return value;
    }

    @SuppressWarnings("WeakerAccess")
    public void showInterstitial(@NonNull String placementId) {
        _logger.debug("showInterstitial");
        com.ironsource.mediationsdk.IronSource.showInterstitial();
    }
}
