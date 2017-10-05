package com.ee.applovin;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Debug;
import android.support.annotation.NonNull;
import android.util.Log;

import com.applovin.adview.AppLovinIncentivizedInterstitial;
import com.applovin.adview.AppLovinInterstitialAd;
import com.applovin.sdk.AppLovinAd;
import com.applovin.sdk.AppLovinAdDisplayListener;
import com.applovin.sdk.AppLovinAdLoadListener;
import com.applovin.sdk.AppLovinSdk;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.DictionaryUtils;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

import java.util.HashMap;
import java.util.Map;

public class AppLovin implements PluginProtocol {
    private static final String k__ALovinads_initALovinAds       = "k__ALovinads_initALovinAds";
    private static final String k__ALovinads_isInterstitialReady = "k__ALovinads_isInterstitialReady";
    private static final String k__ALovinads_showInterstitial    = "k__ALovinads_showInterstitial";
    private static final String k__ALovinads_isRewardVideoReady  = "k__ALovinads_isRewardVideoReady";
    private static final String k__ALovinads_showRewardVideo     = "k__ALovinads_showRewardVideo";
    private static final String k__AppLovin_cpp_callback         = "__ALovinAds_callback";

    private static final Logger _logger = new Logger(AppLovin.class.getName());

    private Activity _context;

    private AppLovinIncentivizedInterstitial _incentive;

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
            public String handle(@NonNull String msg) {
                initialize();
                return "";
            }
        }, k__ALovinads_initALovinAds);
        
        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                return isInterstitialReady() ? "true" : "false";
            }
        }, k__ALovinads_isInterstitialReady);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                return showInterstitial() ? "true" : "false";
            }
        }, k__ALovinads_showInterstitial);
        
        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                return isRewardVideoReady() ? "true" : "false";
            }
        }, k__ALovinads_isRewardVideoReady);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                return showRewardVideo() ? "true" : "false";
            }
        }, k__ALovinads_showRewardVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__ALovinads_initALovinAds);
        bridge.deregisterHandler(k__ALovinads_isInterstitialReady);
        bridge.deregisterHandler(k__ALovinads_showInterstitial);
        bridge.deregisterHandler(k__ALovinads_isRewardVideoReady);
        bridge.deregisterHandler(k__ALovinads_showRewardVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize() {
        AppLovinSdk.initializeSdk(_context);

        _incentive = AppLovinIncentivizedInterstitial.create(_context);

        // Preload call using a new load listener
        _incentive.preload(new AppLovinAdLoadListener() {
            @Override
            public void adReceived(AppLovinAd appLovinAd) {
                // A rewarded video was successfully received.
                _logger.debug("adReceived");
            }
            @Override
            public void failedToReceiveAd(int errorCode) {
                // A rewarded video failed to load.
                _logger.debug("failedToReceiveAd: code " + errorCode);

                Map<String, Object> dict = new HashMap<>();
                dict.put("code", 0);
                dict.put("placement", "videoReward");

                MessageBridge.getInstance().callCpp(k__AppLovin_cpp_callback, JsonUtils.convertDictionaryToString(dict));
            }
        });
    }
    
    @SuppressWarnings("WeakerAccess")
    public boolean isInterstitialReady() {
        return AppLovinInterstitialAd.isAdReadyToDisplay(_context);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showInterstitial() {
        if (!isInterstitialReady()) {
            return false;
        }
        AppLovinInterstitialAd.show(_context);
        return true;
    }
    
    @SuppressWarnings("WeakerAccess")
    public boolean isRewardVideoReady() {
        return _incentive.isAdReadyToDisplay();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showRewardVideo() {
        if (!isRewardVideoReady()) {
            return false;
        }
        _incentive.show(_context, null, null, new AppLovinAdDisplayListener() {
            @Override
            public void adDisplayed(AppLovinAd appLovinAd) {
                // A rewarded video is being displayed.
            }
            @Override
            public void adHidden(AppLovinAd appLovinAd) {
                Map<String, Object> dict = new HashMap<>();
                dict.put("code", 2);
                dict.put("placement", "videoReward");

                MessageBridge .getInstance().callCpp(k__AppLovin_cpp_callback, JsonUtils.convertDictionaryToString(dict));
                // A rewarded video was closed.  Preload the next video now.  We won't use a load listener.
                // FIXME: should be called manually.
                _incentive.preload(null);
            }
        });
        return true;
    }
}
