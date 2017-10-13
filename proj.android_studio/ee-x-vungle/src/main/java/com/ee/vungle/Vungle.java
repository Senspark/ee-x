package com.ee.vungle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
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
        Utils.checkMainThread();
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
        Utils.checkMainThread();
        VunglePub.getInstance().onResume();
    }

    @Override
    public void onPause() {
        Utils.checkMainThread();
        VunglePub.getInstance().onPause();
    }

    @Override
    public void onDestroy() {
        Utils.checkMainThread();
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
        Utils.checkMainThread();
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
                return Utils.toString(hasRewardedVideo());
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
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__hasRewardedVideo);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(final @NonNull String gameId) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }
        VunglePub.getInstance().init(_context, gameId);
        VunglePub.getInstance().setEventListeners(new EventListener() {
            @Override
            public void onAdStart() {
                _logger.info("onAdStart");
                Utils.checkMainThread();

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onStart);
            }

            @Override
            public void onAdEnd(boolean wasSuccessfulView, boolean wasCallToActionClicked) {
                _logger.info("onAdEnd: successful = " + wasSuccessfulView + " clicked = " +
                             wasCallToActionClicked);
                Utils.checkMainThread();

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onEnd, Utils.toString(wasSuccessfulView));
            }

            @Override
            public void onAdPlayableChanged(boolean isAdPlayable) {
                _logger.info("onAdPlayabledChanged: " + isAdPlayable);
                Utils.checkMainThread();
            }

            @Override
            public void onAdUnavailable(String reason) {
                _logger.info("onAdUnavailable: " + reason);
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onUnavailable);
            }
        });
        _initialized = true;
    }

    private void destroy() {
        Utils.checkMainThread();
        if (!_initialized) {
            return;
        }
        VunglePub.getInstance().clearEventListeners();
    }

    private boolean hasRewardedVideo() {
        Utils.checkMainThread();
        return VunglePub.getInstance().isAdPlayable();
    }

    private void showRewardedVideo() {
        Utils.checkMainThread();
        VunglePub.getInstance().playAd();
    }
}
