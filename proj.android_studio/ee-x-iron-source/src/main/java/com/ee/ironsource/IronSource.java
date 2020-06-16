package com.ee.ironsource;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.IPlugin;
import com.ee.core.Logger;
import com.ee.core.internal.Thread;
import com.ee.core.internal.Utils;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.InterstitialListener;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class IronSource implements IPlugin, RewardedVideoListener, InterstitialListener {
    private static final String kPrefix = "IronSource";

    private static final String k__initialize = kPrefix + "_initialize";

    private static final String k__loadInterstitialAd = kPrefix + "_loadInterstitialAd";
    private static final String k__hasInterstitialAd = kPrefix + "_hasInterstitialAd";
    private static final String k__showInterstitialAd = kPrefix + "_showInterstitialAd";

    private static final String k__hasRewardedAd = kPrefix + "_hasRewardedAd";
    private static final String k__showRewardedAd = kPrefix + "_showRewardedAd";

    private static final String k__onInterstitialAdLoaded = kPrefix + "_onInterstitialAdLoaded";
    private static final String k__onInterstitialAdFailedToLoad = kPrefix + "_onInterstitialAdFailedToLoad";
    private static final String k__onInterstitialAdFailedToShow = kPrefix + "_onInterstitialAdFailedToShow";
    private static final String k__onInterstitialAdClicked = kPrefix + "_onInterstitialAdClicked";
    private static final String k__onInterstitialAdClosed = kPrefix + "_onInterstitialAdClosed";

    private static final String k__onRewardedAdLoaded = kPrefix + "_onRewardedAdLoaded";
    private static final String k__onRewardedAdFailedToShow = kPrefix + "_onRewardedAdFailedToShow";
    private static final String k__onRewardedAdClicked = kPrefix + "_onRewardedAdClicked";
    private static final String k__onRewardedAdClosed = kPrefix + "_onRewardedAdClosed";

    private static final Logger _logger = new Logger(IronSource.class.getName());

    private Activity _activity;
    private IMessageBridge _bridge;
    private boolean _initialized;
    private boolean _rewarded;

    public IronSource(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Thread.checkMainThread();
        _logger.debug("constructor begin.");
        _bridge = bridge;
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
        Thread.checkMainThread();
        com.ironsource.mediationsdk.IronSource.onResume(_activity);
    }

    @Override
    public void onPause() {
        Thread.checkMainThread();
        com.ironsource.mediationsdk.IronSource.onPause(_activity);
    }

    @Override
    public void onDestroy() {
    }

    @Override
    public void destroy() {
        Thread.checkMainThread();
        deregisterHandlers();
        _bridge = null;
        if (!_initialized) {
            return;
        }
        com.ironsource.mediationsdk.IronSource.setInterstitialListener(null);
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(null);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__initialize, message -> {
            assertThat(_activity).isNotNull();
            @SuppressWarnings("UnnecessaryLocalVariable")
            String gameId = message;
            initialize(_activity, gameId);
            return "";
        });

        _bridge.registerHandler(k__loadInterstitialAd, message -> {
            loadInterstitialAd();
            return "";
        });

        _bridge.registerHandler(k__hasInterstitialAd, message ->
            Utils.toString(hasInterstitialAd()));

        _bridge.registerHandler(k__showInterstitialAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            showInterstitialAd(adId);
            return "";
        });

        _bridge.registerHandler(k__hasRewardedAd, message ->
            Utils.toString(hasRewardedAd()));

        _bridge.registerHandler(k__showRewardedAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            showRewardedAd(adId);
            return "";
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);

        _bridge.deregisterHandler(k__loadInterstitialAd);
        _bridge.deregisterHandler(k__hasInterstitialAd);
        _bridge.deregisterHandler(k__showInterstitialAd);

        _bridge.deregisterHandler(k__hasRewardedAd);
        _bridge.deregisterHandler(k__showRewardedAd);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull Activity activity, @NonNull String gameId) {
        Thread.checkMainThread();
        if (_initialized) {
            return;
        }
        com.ironsource.mediationsdk.IronSource.init(activity, gameId,
            com.ironsource.mediationsdk.IronSource.AD_UNIT.REWARDED_VIDEO,
            com.ironsource.mediationsdk.IronSource.AD_UNIT.INTERSTITIAL);
        com.ironsource.mediationsdk.IronSource.shouldTrackNetworkState(activity, true);
        com.ironsource.mediationsdk.IronSource.setInterstitialListener(this);
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(this);
        _initialized = true;
    }

    @SuppressWarnings("WeakerAccess")
    public void loadInterstitialAd() {
        Thread.checkMainThread();
        _logger.debug("loadInterstitialAd");
        com.ironsource.mediationsdk.IronSource.loadInterstitial();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasInterstitialAd() {
        Thread.checkMainThread();
        boolean value = com.ironsource.mediationsdk.IronSource.isInterstitialReady();
        _logger.debug("hasInterstitialAd " + value);
        return value;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showInterstitialAd(@NonNull String adId) {
        Thread.checkMainThread();
        _logger.debug("showInterstitialAd");
        com.ironsource.mediationsdk.IronSource.showInterstitial(adId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedAd() {
        Thread.checkMainThread();
        boolean value = com.ironsource.mediationsdk.IronSource.isRewardedVideoAvailable();
        _logger.debug("hasRewardedAd " + value);
        return value;
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedAd(@NonNull String adId) {
        Thread.checkMainThread();
        _logger.debug("showRewardedAd");
        _rewarded = false;
        com.ironsource.mediationsdk.IronSource.showRewardedVideo(adId);
    }

    private void handleRewardedAdResult() {
        _bridge.callCpp(k__onRewardedAdClosed, Utils.toString(_rewarded));
    }

    @Override
    public void onInterstitialAdReady() {
        _logger.debug("onInterstitialAdReady");
        _bridge.callCpp(k__onInterstitialAdLoaded);
    }

    @Override
    public void onInterstitialAdLoadFailed(IronSourceError ironSourceError) {
        _logger.debug("onInterstitialAdLoadFailed");
        _bridge.callCpp(k__onInterstitialAdFailedToLoad, ironSourceError.getErrorMessage());
    }

    @Override
    public void onInterstitialAdOpened() {
        _logger.debug("onInterstitialAdOpened");
    }

    @Override
    public void onInterstitialAdClosed() {
        _logger.debug("onInterstitialAdClosed");
        _bridge.callCpp(k__onInterstitialAdClosed);
    }

    @Override
    public void onInterstitialAdShowSucceeded() {
        _logger.debug("onInterstitialAdShowSucceeded");
    }

    @Override
    public void onInterstitialAdShowFailed(IronSourceError ironSourceError) {
        _logger.debug("onInterstitialAdShowFailed");
        _bridge.callCpp(k__onInterstitialAdFailedToShow, ironSourceError.getErrorMessage());
    }

    @Override
    public void onInterstitialAdClicked() {
        _logger.debug("onInterstitialAdClicked");
        _bridge.callCpp(k__onInterstitialAdClicked);
    }

    @Override
    public void onRewardedVideoAvailabilityChanged(boolean available) {
        _logger.info("onRewardedVideoAvailabilityChanged: " + available);
        if (available) {
            _bridge.callCpp(k__onRewardedAdLoaded);
        }
    }

    @Override
    public void onRewardedVideoAdRewarded(Placement placement) {
        _logger.debug("onRewardedVideoAdRewarded: " + placement.getPlacementName());
        _rewarded = true;
    }

    @Override
    public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {
        _logger.debug("onRewardedVideoAdShowFailed: " + ironSourceError.getErrorMessage());
        _bridge.callCpp(k__onRewardedAdFailedToShow, ironSourceError.getErrorMessage());
    }

    @Override
    public void onRewardedVideoAdClicked(Placement placement) {
        _logger.debug("onRewardedVideoAdClicked");
        _bridge.callCpp(k__onRewardedAdClicked);
    }

    @Override
    public void onRewardedVideoAdOpened() {
        _logger.debug("onRewardedVideoAdOpened");
    }

    @Override
    public void onRewardedVideoAdClosed() {
        _logger.debug("onRewardedVideoAdClosed");
        if (_rewarded) {
            handleRewardedAdResult();
        } else {
            // Note: The onRewardedVideoAdRewarded and onRewardedVideoAdClosed events are
            // asynchronous.
            Thread.runOnMainThreadDelayed(1.0f, this::handleRewardedAdResult);
        }
    }

    @Override
    public void onRewardedVideoAdStarted() {
        _logger.debug("onRewardedVideoAdStarted");
    }

    @Override
    public void onRewardedVideoAdEnded() {
        _logger.debug("onRewardedVideoAdEnded");
    }
}
