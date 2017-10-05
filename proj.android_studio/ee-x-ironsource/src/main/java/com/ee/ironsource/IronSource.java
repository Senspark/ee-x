package com.ee.ironsource;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class IronSource implements PluginProtocol, RewardedVideoListener {
    private static final String k__initialize        = "IronSource_initialize";
    private static final String k__cppCallback       = "IronSource_cppCallback";
    private static final String k__showRewardedVideo = "IronSource_showRewardedVideo";

    private static final Logger _logger = new Logger(IronSource.class.getName());

    private Activity _context;
    private String   _currentPlacementId;

    public IronSource(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "IronSource";
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
                return showRewardedVideo(placementId) ? "true" : "false";
            }
        }, k__showRewardedVideo);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String gameId) {
        com.ironsource.mediationsdk.IronSource.init(_context, gameId,
            com.ironsource.mediationsdk.IronSource.AD_UNIT.REWARDED_VIDEO);
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(this);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isRewardedVideoReady() {
        return com.ironsource.mediationsdk.IronSource.isRewardedVideoAvailable();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showRewardedVideo(@NonNull String placementId) {
        if (!isRewardedVideoReady()) {
            return false;
        }
        _currentPlacementId = placementId;
        com.ironsource.mediationsdk.IronSource.showRewardedVideo(placementId);
        return true;
    }

    private void sendResult(boolean result, @NonNull String placementId) {
        Map<String, Object> dict = new HashMap<>();
        dict.put("result", result);
        dict.put("placementId", placementId);

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__cppCallback, JsonUtils.convertDictionaryToString(dict));
    }

    @Override
    public void onRewardedVideoAvailabilityChanged(boolean available) {
        _logger.info("onRewardedVideoAvailabilityChanged: " + available);
    }

    @Override
    public void onRewardedVideoAdRewarded(Placement placement) {
        _logger.debug("onRewardedVideoAdRewarded: " + placement.getPlacementName());
        sendResult(true, placement.getPlacementName());
    }

    @Override
    public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {
        _logger.debug("onRewardedVideoAdShowFailed: " + ironSourceError.getErrorMessage());
        sendResult(false, _currentPlacementId);
    }

    @Override
    public void onRewardedVideoAdOpened() {
    }

    @Override
    public void onRewardedVideoAdClosed() {
    }

    @Override
    public void onRewardedVideoAdStarted() {
    }

    @Override
    public void onRewardedVideoAdEnded() {
    }
}
