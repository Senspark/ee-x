package com.ee.unityads;

import android.app.Activity;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
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

    private static final String k__gameId          = "gameId";
    private static final String k__testModeEnabled = "testModeEnabled";

    private static final Logger _logger = new Logger(UnityAds.class.getName());

    private Activity _activity;
    private boolean  _initialized;

    @SuppressWarnings("unused")
    public UnityAds() {
        Utils.checkMainThread();
        _logger.debug("constructor begin.");
        _activity = null;
        _initialized = false;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "UnityAds";
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
        if (!_initialized) {
            return;
        }
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
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;

                String gameId = (String) dict.get(k__gameId);
                boolean testModeEnabled = Utils.toBoolean((String) dict.get(k__testModeEnabled));
                assert _activity != null;
                initialize(_activity, gameId, testModeEnabled);
                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setDebugModeEnabled(Utils.toBoolean(message));
                return "";
            }
        }, k__setDebugModeEnabled);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(isRewardedVideoReady(placementId));
            }
        }, k__isRewardedVideoReady);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                assert _activity != null;
                String placementId = message;
                showRewardedVideo(_activity, placementId);
                return "";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__setDebugModeEnabled);
        bridge.deregisterHandler(k__isRewardedVideoReady);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull Activity activity, @NonNull String gameId,
                           boolean testModeEnabled) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }
        com.unity3d.ads.UnityAds.initialize(activity, gameId, new IUnityAdsListener() {
            public void onUnityAdsReady(String placementId) {
                _logger.info("onUnityAdsReady: " + placementId);
                Utils.checkMainThread();
            }

            @Override
            public void onUnityAdsStart(String placementId) {
                _logger.info("onUnityAdsStart: " + placementId);
                Utils.checkMainThread();
            }

            @Override
            public void onUnityAdsFinish(String placementId, FinishState finishState) {
                _logger.info("onUnityAdsFinish: " + placementId + " state = " + finishState);
                Utils.checkMainThread();

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
                Utils.checkMainThread();
            }
        }, testModeEnabled);
        _initialized = true;
    }

    @SuppressWarnings({"unused", "WeakerAccess"})
    public void setDebugModeEnabled(boolean enabled) {
        Utils.checkMainThread();
        com.unity3d.ads.UnityAds.setDebugMode(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isRewardedVideoReady(@NonNull String placementId) {
        Utils.checkMainThread();
        return com.unity3d.ads.UnityAds.isReady(placementId);
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo(@NonNull Activity activity, @NonNull String placementId) {
        Utils.checkMainThread();
        com.unity3d.ads.UnityAds.show(activity, placementId);
    }
}