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

import java.util.HashMap;
import java.util.Map;

public class UnityAds implements PluginProtocol {
    private static final String k__initialize        = "UnityAds_initialize";
    private static final String k__cppCallback       = "UnityAds_cppCallback";
    private static final String k__showRewardedVideo = "UnityAds_showRewardedVideo";

    private static final Logger _logger = new Logger(UnityAds.class.getName());

    private Activity _context;
    private boolean  _playAdSuccessfully;

    public UnityAds(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
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
                String gameId = message;
                initialize(gameId);
                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return (showRewardedVideo(placementId)) ? "true" : "false";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(final @NonNull String gameId) {
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

                Map<String, Object> dict = new HashMap<>();
                if (finishState == FinishState.COMPLETED) {
                    dict.put("result", true);
                } else {
                    dict.put("result", false);
                }
                dict.put("placementId", placementId);

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__cppCallback, JsonUtils.convertDictionaryToString(dict));
            }

            @Override
            public void onUnityAdsError(UnityAdsError unityAdsError, String s) {
                _logger.info("onUnityAdsError: " + s + " error = " + unityAdsError);
                _playAdSuccessfully = false;
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isRewardedVideoReady(final @NonNull String placementId) {
        return com.unity3d.ads.UnityAds.isReady(placementId);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showRewardedVideo(final @NonNull String placementId) {
        _logger.info("showRewardedVideo: begin");
        if (!isRewardedVideoReady(placementId)) {
            return false;
        }
        _playAdSuccessfully = true;
        com.unity3d.ads.UnityAds.show(_context, placementId);
        _logger.info("showRewardedVideo: end");
        return _playAdSuccessfully;
    }
}