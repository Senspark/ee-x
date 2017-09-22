package com.ee.alovinads;

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

public class ALovinAds implements PluginProtocol {
    private static final String k__ALovinads_initALovinAds = "k__ALovinads_initALovinAds";
    private static final String k__ALovinads_isInterstitialReady   = "k__ALovinads_isInterstitialReady";
    private static final String k__ALovinads_showInterstitial      = "k__ALovinads_showInterstitial";
    private static final String k__ALovinads_isRewardVideoReady   = "k__ALovinads_isRewardVideoReady";
    private static final String k__ALovinads_showRewardVideo      = "k__ALovinads_showRewardVideo";

    private static final Logger _logger = new Logger(ALovinAds.class.getName());

    private Activity          _context;

    private AppLovinIncentivizedInterstitial _incentive;

    public ALovinAds(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;

        registerHandlers();

        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "AlovinAds";
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

                initUnityAds();
                return DictionaryUtils.emptyResult();
            }
        }, k__ALovinads_initALovinAds);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                showInterstitial();
                return DictionaryUtils.emptyResult();
            }
        }, k__ALovinads_showInterstitial);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                return (isInterstitialReady()) ? "true" : "false";
            }
        }, k__ALovinads_isInterstitialReady);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                showRewardVideo();
                return DictionaryUtils.emptyResult();
            }
        }, k__ALovinads_showRewardVideo);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                return (isRewardVideoReady()) ? "true" : "false";
            }
        }, k__ALovinads_isRewardVideoReady);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__ALovinads_initALovinAds);
        bridge.deregisterHandler(k__ALovinads_showInterstitial);
        bridge.deregisterHandler(k__ALovinads_isInterstitialReady);

        bridge.deregisterHandler(k__ALovinads_showRewardVideo);
        bridge.deregisterHandler(k__ALovinads_isRewardVideoReady);
    }

    @SuppressWarnings("WeakerAccess")
    public void initUnityAds() {
        AppLovinSdk.initializeSdk(_context);

        _incentive = AppLovinIncentivizedInterstitial.create(_context);

        // Preload call using a new load listener
        _incentive.preload(new AppLovinAdLoadListener() {
            @Override
            public void adReceived(AppLovinAd appLovinAd) {
                // A rewarded video was successfully received.
                Log.d("EEAppLovin", "adReceived  ");
            }
            @Override
            public void failedToReceiveAd(int errorCode) {
                // A rewarded video failed to load.
                Log.d("EEAppLovin", "failedToReceiveAd code " + errorCode );

                Map<String, Object> dict = new HashMap<>();
                dict.put("code", 0);
                dict.put("placement", "videoReward");

                MessageBridge.getInstance().callCpp("__ALovinAds_callback", JsonUtils.convertDictionaryToString(dict));
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public void showInterstitial() {
        AppLovinInterstitialAd.show(_context);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isInterstitialReady() {
        return AppLovinInterstitialAd.isAdReadyToDisplay(_context);
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardVideo() {
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

                MessageBridge .getInstance().callCpp("__ALovinAds_callback", JsonUtils.convertDictionaryToString(dict));
                // A rewarded video was closed.  Preload the next video now.  We won't use a load listener.
                _incentive.preload(null);
            }
        });;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isRewardVideoReady() {
        return _incentive.isAdReadyToDisplay();
    }
}