package com.ee.applovin;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.applovin.adview.AppLovinIncentivizedInterstitial;
import com.applovin.adview.AppLovinInterstitialAd;
import com.applovin.sdk.AppLovinAd;
import com.applovin.sdk.AppLovinAdDisplayListener;
import com.applovin.sdk.AppLovinAdLoadListener;
import com.applovin.sdk.AppLovinAdRewardListener;
import com.applovin.sdk.AppLovinAdSize;
import com.applovin.sdk.AppLovinSdk;
import com.applovin.sdk.AppLovinSdkSettings;
import com.applovin.sdk.AppLovinSdkUtils;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

import java.util.Map;

public class AppLovin implements PluginProtocol {
    private static final String k__initialize               = "AppLovin_initialize";
    private static final String k__setTestAdsEnabled        = "AppLovin_setTestAdsEnabled";
    private static final String k__setVerboseLogging        = "AppLovin_setVerboseLogging";
    private static final String k__setMuted                 = "AppLovin_setMuted";
    private static final String k__hasInterstitialAd        = "AppLovin_hasInterstitialAd";
    private static final String k__showInterstitialAd       = "AppLovin_showInterstitialAd";
    private static final String k__loadRewardedVideo        = "AppLovin_loadRewardedVideo";
    private static final String k__hasRewardedVideo         = "AppLovin_hasRewardedVideo";
    private static final String k__showRewardedVideo        = "AppLovin_showRewardedVideo";
    private static final String k__onInterstitialAdHidden   = "AppLovin_onInterstitialAdHidden";
    private static final String k__onRewardedVideoFailed    = "AppLovin_onRewardedVideoFailed";
    private static final String k__onRewardedVideoDisplayed = "AppLovin_onRewardedVideoDisplayed";
    private static final String k__onRewardedVideoHidden    = "AppLovin_onRewardedVideoHidden";
    private static final String k__onUserRewardVerified     = "AppLovin_onUserRewardVerified";

    private static final Logger _logger = new Logger(AppLovin.class.getName());

    private Activity _context;

    private AppLovinSdk                      _sdk;
    private AppLovinIncentivizedInterstitial _incentivizedInterstitialAd;
    private AppLovinAdLoadListener           _incentivizedInterstitialAdLoadListener;
    private AppLovinAdRewardListener         _incentivizedInterstitialAdRewardListener;
    private AppLovinAdDisplayListener        _incentivizedInterstitialAdDisplayListener;

    public AppLovin(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "AppLovin";
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
        deregisterHandlers();
        _sdk = null;
        _incentivizedInterstitialAd = null;
        _incentivizedInterstitialAdLoadListener = null;
        _incentivizedInterstitialAdRewardListener = null;
        _incentivizedInterstitialAdDisplayListener = null;
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
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String key = message;
                initialize(key);
                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setTestAdEnabled(message.equals("true"));
                return "";
            }
        }, k__setTestAdsEnabled);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setVerboseLoggin(message.equals("true"));
                return "";
            }
        }, k__setVerboseLogging);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setMuted(message.equals("true"));
                return "";
            }
        }, k__setMuted);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return hasInterstitialAd() ? "true" : "false";
            }
        }, k__hasInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showInterstitialAd();
                return "";
            }
        }, k__showInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                loadRewardedVideo();
                return "";
            }
        }, k__loadRewardedVideo);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return hasRewardedVideo() ? "true" : "false";
            }
        }, k__hasRewardedVideo);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showRewardedVideo();
                return "";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__setTestAdsEnabled);
        bridge.deregisterHandler(k__setVerboseLogging);
        bridge.deregisterHandler(k__setMuted);
        bridge.deregisterHandler(k__hasInterstitialAd);
        bridge.deregisterHandler(k__showInterstitialAd);
        bridge.deregisterHandler(k__loadRewardedVideo);
        bridge.deregisterHandler(k__hasRewardedVideo);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String key) {
        AppLovinSdkSettings settings = AppLovinSdkUtils.retrieveUserSettings(_context);
        _sdk = AppLovinSdk.getInstance(key, settings, _context);
        _sdk.initializeSdk();

        _incentivizedInterstitialAd = AppLovinIncentivizedInterstitial.create(_sdk);

        _incentivizedInterstitialAdLoadListener = new AppLovinAdLoadListener() {
            @Override
            public void adReceived(AppLovinAd ad) {
                _logger.info("adReceived");
            }

            @Override
            public void failedToReceiveAd(int errorCode) {
                _logger.info("failedToReceiveAd: code " + errorCode);
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onRewardedVideoFailed, String.valueOf(errorCode));
            }
        };

        _incentivizedInterstitialAdRewardListener = new AppLovinAdRewardListener() {
            @Override
            public void userRewardVerified(AppLovinAd ad, Map<String, String> response) {
                _logger.info("userRewardVerified");
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onUserRewardVerified);
            }

            @Override
            public void userOverQuota(AppLovinAd ad, Map<String, String> response) {
                _logger.info("userOverQuota");
            }

            @Override
            public void userRewardRejected(AppLovinAd ad, Map<String, String> response) {
                _logger.info("userRewardRejected");
            }

            @Override
            public void validationRequestFailed(AppLovinAd ad, int responseCode) {
                _logger.info("validationRequestFailed: code = " + responseCode);
            }

            @Override
            public void userDeclinedToViewAd(AppLovinAd ad) {
                _logger.info("userDeclinedToViewAd");
            }
        };

        _incentivizedInterstitialAdDisplayListener = new AppLovinAdDisplayListener() {
            @Override
            public void adDisplayed(AppLovinAd ad) {
                _logger.info("adDisplayed");
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onRewardedVideoDisplayed);
            }

            @Override
            public void adHidden(AppLovinAd ad) {
                _logger.info("adHidden");
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onRewardedVideoHidden);
            }
        };
    }

    @SuppressWarnings("WeakerAccess")
    public void setTestAdEnabled(boolean enabled) {
        _sdk.getSettings().setTestAdsEnabled(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public void setVerboseLoggin(boolean enabled) {
        _sdk.getSettings().setVerboseLogging(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public void setMuted(boolean enabled) {
        _sdk.getSettings().setMuted(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasInterstitialAd() {
        return _sdk.getAdService().hasPreloadedAd(AppLovinAdSize.INTERSTITIAL);
    }

    @SuppressWarnings("WeakerAccess")
    public void showInterstitialAd() {
        AppLovinInterstitialAd.show(_sdk, _context, null);
    }

    @SuppressWarnings("WeakerAccess")
    public void loadRewardedVideo() {
        _incentivizedInterstitialAd.preload(_incentivizedInterstitialAdLoadListener);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedVideo() {
        return _incentivizedInterstitialAd.isAdReadyToDisplay();
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo() {
        _incentivizedInterstitialAd.show(_context, _incentivizedInterstitialAdRewardListener, null,
            _incentivizedInterstitialAdDisplayListener);
    }
}
