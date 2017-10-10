package com.ee.ironsource;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class IronSource implements PluginProtocol {
    private static final String k__initialize        = "IronSource_initialize";
    private static final String k__hasRewardedVideo  = "IronSource_hasRewardedVideo";
    private static final String k__showRewardedVideo = "IronSource_showRewardedVideo";
    private static final String k__onRewarded        = "IronSource_onRewarded";
    private static final String k__onFailed          = "IronSource_onFailed";
    private static final String k__onOpened          = "IronSource_onOpened";
    private static final String k__onClosed          = "IronSource_onClosed";

    private static final Logger _logger = new Logger(IronSource.class.getName());

    private Activity _context;

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
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(null);
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
                return hasRewardedVideo() ? "true" : "false";
            }
        }, k__hasRewardedVideo);

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
        bridge.deregisterHandler(k__hasRewardedVideo);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String gameId) {
        com.ironsource.mediationsdk.IronSource.init(_context, gameId,
            com.ironsource.mediationsdk.IronSource.AD_UNIT.REWARDED_VIDEO);
        com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(
            new RewardedVideoListener() {
                @Override
                public void onRewardedVideoAvailabilityChanged(boolean available) {
                    _logger.info("onRewardedVideoAvailabilityChanged: " + available);
                }

                @Override
                public void onRewardedVideoAdRewarded(Placement placement) {
                    _logger.debug("onRewardedVideoAdRewarded: " + placement.getPlacementName());
                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onRewarded, placement.getPlacementName());
                }

                @Override
                public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {
                    _logger.debug(
                        "onRewardedVideoAdShowFailed: " + ironSourceError.getErrorMessage());
                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onFailed);
                }

                @Override
                public void onRewardedVideoAdOpened() {
                    _logger.debug("onRewardedVideoAdOpened");
                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onOpened);
                }

                @Override
                public void onRewardedVideoAdClosed() {
                    _logger.debug("onRewardedVideoAdClosed");
                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onClosed);
                }

                @Override
                public void onRewardedVideoAdStarted() {
                    _logger.debug("onRewardedVideoAdStarted");
                }

                @Override
                public void onRewardedVideoAdEnded() {
                    _logger.debug("onRewardedVideoAdEnded");
                }
            });
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedVideo() {
        return com.ironsource.mediationsdk.IronSource.isRewardedVideoAvailable();
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo(@NonNull String placementId) {
        com.ironsource.mediationsdk.IronSource.showRewardedVideo(placementId);
    }
}
