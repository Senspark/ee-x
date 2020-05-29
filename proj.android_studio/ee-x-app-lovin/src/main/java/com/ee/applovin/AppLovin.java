package com.ee.applovin;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.applovin.adview.AppLovinIncentivizedInterstitial;
import com.applovin.adview.AppLovinInterstitialAd;
import com.applovin.adview.AppLovinInterstitialAdDialog;
import com.applovin.sdk.AppLovinAd;
import com.applovin.sdk.AppLovinAdClickListener;
import com.applovin.sdk.AppLovinAdDisplayListener;
import com.applovin.sdk.AppLovinAdLoadListener;
import com.applovin.sdk.AppLovinAdRewardListener;
import com.applovin.sdk.AppLovinAdSize;
import com.applovin.sdk.AppLovinSdk;
import com.applovin.sdk.AppLovinSdkSettings;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;

import java.util.Map;

public class AppLovin implements PluginProtocol {
    private static final String kPrefix = "AppLovin";

    private static final String k__initialize = kPrefix + "_initialize";
    private static final String k__setTestAdsEnabled = kPrefix + "_setTestAdsEnabled";
    private static final String k__setVerboseLogging = kPrefix + "_setVerboseLogging";
    private static final String k__setMuted = kPrefix + "_setMuted";

    private static final String k__hasInterstitialAd = kPrefix + "_hasInterstitialAd";
    private static final String k__loadInterstitialAd = kPrefix + "_loadInterstitialAd";
    private static final String k__showInterstitialAd = kPrefix + "_showInterstitialAd";

    private static final String k__hasRewardedAd = kPrefix + "_hasRewardedAd";
    private static final String k__loadRewardedAd = kPrefix + "_loadRewardedAd";
    private static final String k__showRewardedAd = kPrefix + "_showRewardedAd";

    private static final String k__onInterstitialAdLoaded = kPrefix + "_onInterstitialAdLoaded";
    private static final String k__onInterstitialAdFailedToLoad = kPrefix + "_onInterstitialAdFailedToLoad";
    private static final String k__onInterstitialAdClicked = kPrefix + "_onInterstitialAdClicked";
    private static final String k__onInterstitialAdClosed = kPrefix + "_onInterstitialAdClosed";

    private static final String k__onRewardedAdLoaded = kPrefix + "_onRewardedAdLoaded";
    private static final String k__onRewardedAdFailedToLoad = kPrefix + "_onRewardedAdFailedToLoad";
    private static final String k__onRewardedAdClicked = kPrefix + "_onRewardedAdClicked";
    private static final String k__onRewardedAdClosed = kPrefix + "_onRewardedAdClosed";

    private static final Logger _logger = new Logger(AppLovin.class.getName());

    private class InterstitialAdListener implements AppLovinAdLoadListener, AppLovinAdDisplayListener, AppLovinAdClickListener {
        @Override
        public void adReceived(AppLovinAd ad) {
            _logger.info("adReceived");
            _bridge.callCpp(k__onInterstitialAdLoaded);
        }

        @Override
        public void failedToReceiveAd(int errorCode) {
            _logger.info("failedToReceiveAd: code " + errorCode);
            _bridge.callCpp(k__onInterstitialAdFailedToLoad, String.valueOf(errorCode));
        }

        @Override
        public void adDisplayed(AppLovinAd ad) {
            _logger.info("adDisplayed");
            Utils.checkMainThread();
        }

        @Override
        public void adHidden(AppLovinAd ad) {
            _logger.info("adHidden");
            Utils.checkMainThread();
            _bridge.callCpp(k__onInterstitialAdClosed);
        }

        @Override
        public void adClicked(AppLovinAd ad) {
            _logger.info("adClicked");
            Utils.checkMainThread();
            _bridge.callCpp(k__onInterstitialAdClicked);
        }
    }

    private class RewardedAdListener implements AppLovinAdLoadListener, AppLovinAdDisplayListener, AppLovinAdRewardListener, AppLovinAdClickListener {
        boolean _rewarded;

        @Override
        public void adReceived(AppLovinAd ad) {
            _logger.info("adReceived");
            _bridge.callCpp(k__onRewardedAdLoaded);
        }

        @Override
        public void failedToReceiveAd(int errorCode) {
            _logger.info("failedToReceiveAd: code " + errorCode);
            _bridge.callCpp(k__onRewardedAdFailedToLoad, String.valueOf(errorCode));
        }

        @Override
        public void adDisplayed(AppLovinAd ad) {
            _logger.info("adDisplayed");
            Utils.checkMainThread();
        }

        @Override
        public void adHidden(AppLovinAd ad) {
            _logger.info("adHidden");
            Utils.checkMainThread();
            _bridge.callCpp(k__onRewardedAdClosed, Utils.toString(_rewarded));
        }

        @Override
        public void userRewardVerified(AppLovinAd ad, Map<String, String> response) {
            _logger.info("userRewardVerified: " + response);
            Utils.checkMainThread();
            _rewarded = true;
        }

        @Override
        public void userOverQuota(AppLovinAd ad, Map<String, String> response) {
            _logger.info("userOverQuota: " + response);
            Utils.checkMainThread();
        }

        @Override
        public void userRewardRejected(AppLovinAd ad, Map<String, String> response) {
            _logger.info("userRewardRejected: " + response);
            Utils.checkMainThread();
        }

        @Override
        public void validationRequestFailed(AppLovinAd ad, int responseCode) {
            _logger.info("validationRequestFailed: code = " + responseCode);
            Utils.checkMainThread();
        }

        @Override
        public void userDeclinedToViewAd(AppLovinAd ad) {
            _logger.info("userDeclinedToViewAd");
            Utils.checkMainThread();
        }

        @Override
        public void adClicked(AppLovinAd ad) {
            _logger.info("adClicked");
            Utils.checkMainThread();
            _bridge.callCpp(k__onRewardedAdClicked);
        }
    }

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private boolean _initialized;
    private AppLovinSdk _sdk;
    private AppLovinInterstitialAdDialog _interstitialAd;
    private InterstitialAdListener _interstitialAdListener;
    private AppLovinIncentivizedInterstitial _rewardedAd;
    private RewardedAdListener _rewardedAdListener;

    public AppLovin(@NonNull Context context, @NonNull IMessageBridge bridge) {
        _logger.debug("constructor begin: context = " + context);
        Utils.checkMainThread();
        _context = context;
        _bridge = bridge;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "AppLovin";
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
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        _context = null;
        _bridge = null;
        if (!_initialized) {
            return;
        }
        _sdk = null;
        _rewardedAd = null;
        _rewardedAdListener = null;
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
        _bridge.registerHandler(message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String key = message;
            initialize(key);
            return "";
        }, k__initialize);

        _bridge.registerHandler(message -> {
            setTestAdEnabled(Utils.toBoolean(message));
            return "";
        }, k__setTestAdsEnabled);

        _bridge.registerHandler(message -> {
            setVerboseLogging(Utils.toBoolean(message));
            return "";
        }, k__setVerboseLogging);

        _bridge.registerHandler(message -> {
            setMuted(Utils.toBoolean(message));
            return "";
        }, k__setMuted);

        _bridge.registerHandler(message -> {
            loadInterstitialAd();
            return "";
        }, k__loadInterstitialAd);

        _bridge.registerHandler(message ->
            Utils.toString(hasInterstitialAd()), k__hasInterstitialAd);

        _bridge.registerHandler(message -> {
            showInterstitialAd();
            return "";
        }, k__showInterstitialAd);

        _bridge.registerHandler(message ->
            Utils.toString(hasRewardedAd()), k__hasRewardedAd);

        _bridge.registerHandler(message -> {
            loadRewardedAd();
            return "";
        }, k__loadRewardedAd);

        _bridge.registerHandler(message -> {
            showRewardedAd();
            return "";
        }, k__showRewardedAd);
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
        _bridge.deregisterHandler(k__setTestAdsEnabled);
        _bridge.deregisterHandler(k__setVerboseLogging);
        _bridge.deregisterHandler(k__setMuted);
        _bridge.deregisterHandler(k__hasInterstitialAd);
        _bridge.deregisterHandler(k__loadInterstitialAd);
        _bridge.deregisterHandler(k__showInterstitialAd);
        _bridge.deregisterHandler(k__hasRewardedAd);
        _bridge.deregisterHandler(k__loadRewardedAd);
        _bridge.deregisterHandler(k__showRewardedAd);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String key) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }

        AppLovinSdkSettings settings = new AppLovinSdkSettings();
        settings.setVerboseLogging(false);

        _sdk = AppLovinSdk.getInstance(key, settings, _context);
        _sdk.initializeSdk();

        _interstitialAd = AppLovinInterstitialAd.create(_sdk, _activity);
        _interstitialAdListener = new InterstitialAdListener();
        _interstitialAd.setAdLoadListener(_interstitialAdListener);
        _interstitialAd.setAdDisplayListener(_interstitialAdListener);
        _interstitialAd.setAdClickListener(_interstitialAdListener);

        _rewardedAd = AppLovinIncentivizedInterstitial.create(_sdk);
        _rewardedAdListener = new RewardedAdListener();
        _initialized = true;
    }

    @SuppressWarnings("WeakerAccess")
    public void setTestAdEnabled(boolean enabled) {
        Utils.checkMainThread();
        // Removed.
    }

    @SuppressWarnings("WeakerAccess")
    public void setVerboseLogging(boolean enabled) {
        Utils.checkMainThread();
        _sdk.getSettings().setVerboseLogging(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public void setMuted(boolean enabled) {
        Utils.checkMainThread();
        _sdk.getSettings().setMuted(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasInterstitialAd() {
        Utils.checkMainThread();
        // FIXME.
        return _sdk.getAdService().hasPreloadedAd(AppLovinAdSize.INTERSTITIAL);
    }

    public void loadInterstitialAd() {
        Utils.checkMainThread();
        _sdk.getAdService().loadNextAd(AppLovinAdSize.INTERSTITIAL, _interstitialAdListener);
    }

    @SuppressWarnings("WeakerAccess")
    public void showInterstitialAd() {
        Utils.checkMainThread();
        _interstitialAd.show();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedAd() {
        Utils.checkMainThread();
        return _rewardedAd.isAdReadyToDisplay();
    }

    @SuppressWarnings("WeakerAccess")
    public void loadRewardedAd() {
        Utils.checkMainThread();
        _rewardedAd.preload(_rewardedAdListener);
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedAd() {
        Utils.checkMainThread();
        _rewardedAdListener._rewarded = false;
        _rewardedAd.show(_context, _rewardedAdListener, null, _rewardedAdListener, _rewardedAdListener);
    }
}
