package com.ee.unityads;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds.FinishState;
import com.unity3d.ads.UnityAds.UnityAdsError;

import java.util.Map;

public class UnityAds implements PluginProtocol {
    private static final String k__initialize           = "UnityAds_initialize";
    private static final String k__setDebugModeEnabled  = "UnityAds_setDebugModeEnabled";
    private static final String k__isRewardedVideoReady = "UnityAds_isRewardedVideoReady";
    private static final String k__showRewardedVideo    = "UnityAds_showRewardedVideo";
    private static final String k__onError              = "UnityAds_onError";
    private static final String k__onSkipped            = "UnityAds_onSkipped";
    private static final String k__onFinished           = "UnityAds_onFinished";

    private static final Logger _logger = new Logger(UnityAds.class.getName());

    private Activity _context;

    @SuppressWarnings("unused")
    public UnityAds(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        _playAdSuccessfully = false;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "UnityAds";
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
        com.unity3d.ads.UnityAds.setListener(null);
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
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;
                String gameId = (String) dict.get("gameId");
                boolean testModeEnabled = dict.get("testModeEnabled").equals("true");
                initialize(gameId, testModeEnabled);
                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setDebugModeEnabled(message.equals("true"));
                return "";
            }
        }, k__setDebugModeEnabled);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return isRewardedVideoReady(placementId) ? "true" : "false";
            }
        }, k__isRewardedVideoReady);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                showRewardedVideo(placementId);
                return "";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__setDebugModeEnabled);
        bridge.deregisterHandler(k__isRewardedVideoReady);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String gameId, boolean testModeEnabled) {
        com.unity3d.ads.UnityAds.initialize(_context, gameId, new IUnityAdsListener() {
            @Override
            public void onUnityAdsReady(String placementId) {
                _logger.info("onUnityAdsReady: " + placementId);
            }

            @Override
            public void onUnityAdsStart(String placementId) {
                _logger.info("onUnityAdsStart: " + placementId);
            }

            @Override
            public void onUnityAdsFinish(String placementId, FinishState finishState) {
                _logger.info("onUnityAdsFinish: " + placementId + " state = " + finishState);

                MessageBridge bridge = MessageBridge.getInstance();
                if (finishState == FinishState.SKIPPED) {
                    bridge.callCpp(k__onError, placementId);
                    return;
                }
                if (finishState == FinishState.SKIPPED) {
                    bridge.callCpp(k__onSkipped, placementId);
                    return;
                }
                if (finishState == FinishState.COMPLETED) {
                    bridge.callCpp(k__onFinished, placementId);
                    return;
                }
                assert false;
            }

            @Override
            public void onUnityAdsError(UnityAdsError unityAdsError, String s) {
                _logger.info("onUnityAdsError: " + s + " error = " + unityAdsError);
            }
        }, testModeEnabled);
    }

    @SuppressWarnings({"unused", "WeakerAccess"})
    public void setDebugModeEnabled(boolean enabled) {
        com.unity3d.ads.UnityAds.setDebugMode(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isRewardedVideoReady(final @NonNull String placementId) {
        return com.unity3d.ads.UnityAds.isReady(placementId);
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo(final @NonNull String placementId) {
        com.unity3d.ads.UnityAds.show(_context, placementId);
    }
}