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
import com.applovin.sdk.AppLovinSdk;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

import java.util.HashMap;
import java.util.Map;

public class AppLovin implements PluginProtocol {
    private static final String k__initialize            = "AppLovin_initialize";
    private static final String k__isInterstitialAdReady = "AppLovin_isInterstitialAdReady";
    private static final String k__showInterstitialAd    = "AppLovin_showInterstitialAd";
    private static final String k__showRewardedVideo     = "AppLovin_showRewardedVideo";
    private static final String k__cppCallback           = "AppLovin_cppCallback";

    private static final Logger _logger = new Logger(AppLovin.class.getName());

    private Activity _context;

    private AppLovinIncentivizedInterstitial _incentivizedInterstitial;
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
        _incentivizedInterstitial = null;
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
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                initialize();
                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return isInterstitialAdReady() ? "true" : "false";
            }
        }, k__isInterstitialAdReady);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return showInterstitialAd() ? "true" : "false";
            }
        }, k__showInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return showRewardedVideo() ? "true" : "false";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__isInterstitialAdReady);
        bridge.deregisterHandler(k__showInterstitialAd);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize() {
        AppLovinSdk.initializeSdk(_context);

        _incentivizedInterstitial = AppLovinIncentivizedInterstitial.create(_context);

        _incentivizedInterstitialAdLoadListener = new AppLovinAdLoadListener() {
            @Override
            public void adReceived(AppLovinAd ad) {
                _logger.info("adReceived");
            }

            @Override
            public void failedToReceiveAd(int errorCode) {
                _logger.info("failedToReceiveAd: code " + errorCode);
                sendResult(true);
            }
        };

        _incentivizedInterstitialAdRewardListener = new AppLovinAdRewardListener() {
            @Override
            public void userRewardVerified(AppLovinAd ad, Map<String, String> response) {
                _logger.info("userRewardVerified");
                sendResult(false);
            }

            @Override
            public void userOverQuota(AppLovinAd ad, Map<String, String> response) {
                _logger.info("userOverQuota");
                sendResult(false);
            }

            @Override
            public void userRewardRejected(AppLovinAd ad, Map<String, String> response) {
                _logger.info("userRewardRejected");
                sendResult(false);
            }

            @Override
            public void validationRequestFailed(AppLovinAd ad, int responseCode) {
                _logger.info("validationRequestFailed: code = " + responseCode);
                sendResult(false);
            }

            @Override
            public void userDeclinedToViewAd(AppLovinAd ad) {
                _logger.info("userDeclinedToViewAd");
                sendResult(false);
            }
        };

        _incentivizedInterstitialAdDisplayListener = new AppLovinAdDisplayListener() {
            @Override
            public void adDisplayed(AppLovinAd ad) {
                _logger.info("adDisplayed");
            }

            @Override
            public void adHidden(AppLovinAd ad) {
                _logger.info("adHidden");
                loadRewardedVideo();
            }
        };

        // Preload call using a new load listener
        loadRewardedVideo();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isInterstitialAdReady() {
        return AppLovinInterstitialAd.isAdReadyToDisplay(_context);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showInterstitialAd() {
        if (!isInterstitialAdReady()) {
            return false;
        }
        AppLovinInterstitialAd.show(_context);
        return true;
    }

    public void loadRewardedVideo() {
        _incentivizedInterstitial.preload(_incentivizedInterstitialAdLoadListener);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isRewardedVideoReady() {
        return _incentivizedInterstitial.isAdReadyToDisplay();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showRewardedVideo() {
        _logger.info("showRewardedVideo: begin");
        if (!isRewardedVideoReady()) {
            loadRewardedVideo();
            return false;
        }
        _incentivizedInterstitial.show(_context, _incentivizedInterstitialAdRewardListener, null,
            _incentivizedInterstitialAdDisplayListener);
        _logger.info("showRewardedVideo: end");
        return true;
    }

    private void sendResult(boolean result) {
        Map<String, Object> dict = new HashMap<>();
        dict.put("result", result);

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__cppCallback, JsonUtils.convertDictionaryToString(dict));
    }
}
