package com.ee.vungle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.vungle.publisher.EventListener;
import com.vungle.publisher.VunglePub;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Vungle implements PluginProtocol {
    private static final String k__initialize        = "Vungle_initialize";
    private static final String k__hasRewardedVideo  = "Vungle_hasRewardedVideo";
    private static final String k__showRewardedVideo = "Vungle_showRewardedVideo";
    private static final String k__onStart           = "Vungle_onStart";
    private static final String k__onEnd             = "Vungle_onEnd";
    private static final String k__onUnavailable     = "Vungle_onUnavailable";

    private static final Logger _logger = new Logger(Vungle.class.getName());

    private Activity _context;
    private boolean  _initialized;

    public Vungle(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        _initialized = false;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Vungle";
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
        VunglePub.getInstance().onResume();
    }

    @Override
    public void onPause() {
        VunglePub.getInstance().onPause();
    }

    @Override
    public void onDestroy() {
        deregisterHandlers();
        destroy();
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
                showRewardedVideo();

                // Compliant with iOS.
                return "true";
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
    public void initialize(final @NonNull String gameId) {
        if (_initialized) {
            return;
        }
        VunglePub.getInstance().init(_context, gameId);
        VunglePub.getInstance().setEventListeners(new EventListener() {
            @Override
            public void onAdStart() {
                // Called before playing an ad
                _logger.info("onAdStart");

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onStart);
            }

            @Override
            public void onAdEnd(boolean wasSuccessfulView, boolean wasCallToActionClicked) {
                // Called when the user leaves the ad and control is returned to your application
                // if wasSuccessfulView is true, the user watched the ad and should be rewarded
                // (if this was a rewarded ad).
                // if wasCallToActionClicked is true, the user clicked the call to action
                // button in the ad.
                _logger.info("onAdEnd: successful = " + wasSuccessfulView + " clicked = " +
                             wasCallToActionClicked);

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onEnd, wasSuccessfulView ? "true" : "false");
            }

            @Override
            public void onAdPlayableChanged(boolean isAdPlayable) {
                // Called when the playability state changes. if isAdPlayable is true, you can now
                // play an ad.
                // If false, you cannot yet play an ad.
                _logger.info("onAdPlayabledChanged: " + isAdPlayable);
            }

            @Override
            public void onAdUnavailable(String reason) {
                // Called when VunglePub.playAd() was called, but no ad was available to play
                _logger.info("onAdUnavailable: " + reason);
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onUnavailable);
            }
        });
        _initialized = true;
    }

    private void destroy() {
        if (!_initialized) {
            return;
        }
        VunglePub.getInstance().clearEventListeners();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedVideo() {
        return VunglePub.getInstance().isAdPlayable();
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo() {
        VunglePub.getInstance().playAd();
    }
}
