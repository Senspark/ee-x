package com.ee.vungle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.vungle.publisher.AdConfig;
import com.vungle.publisher.VungleAdEventListener;
import com.vungle.publisher.VungleInitListener;
import com.vungle.publisher.VunglePub;

import java.util.Map;

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

    private Context _context;
    private boolean _initialized;

    final VunglePub vunglePub = VunglePub.getInstance();
    private static AdConfig globalAdConfig;

    public Vungle(Context context) {
        Utils.checkMainThread();
        _logger.debug("constructor begin: context = " + context);
        _context = context;
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
    public void onCreate(@NonNull Activity activity) {
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
        vunglePub.onResume();
    }

    @Override
    public void onPause() {
        Utils.checkMainThread();
        vunglePub.onPause();
    }

    @Override
    public void onDestroy() {
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        if (!_initialized) {
            return;
        }
        vunglePub.clearEventListeners();
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

                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;

                String gameId = (String) dict.get("gameId");
                String placementId = (String) dict.get("placementId");

                initialize(gameId, placementId);

                return "";
            }
        }, k__initialize);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(hasRewardedVideo(message));
            }
        }, k__hasRewardedVideo);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showRewardedVideo(message);

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
    public void initialize(final @NonNull String gameId, final @NonNull String placementId) {
        Utils.checkMainThread();
        if (_initialized) {
            return;
        }

        vunglePub.init(_context, gameId, new String[] { placementId }, new VungleInitListener()
        {
            @Override
            public void onSuccess() {
                _logger.info("vunglePub.init onSuccess");
                globalAdConfig = vunglePub.getGlobalAdConfig();
            }
            @Override
            public void onFailure(Throwable e){
                _logger.info("vunglePub.init onFailure");
            }
        });
        vunglePub.addEventListeners(new VungleAdEventListener() {
            @Override
            public void onAdEnd(@NonNull String s, boolean wasSuccessfulView, boolean wasCallToActionClicked) {
                _logger.info("onAdEnd: successful = " + wasSuccessfulView + " clicked = " +
                             wasCallToActionClicked);
                Utils.checkMainThread();

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onEnd, Utils.toString(wasSuccessfulView));
            }

            @Override
            public void onAdStart(@NonNull String s) {
                _logger.info("onAdStart");
                Utils.checkMainThread();

                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onStart);
            }

            @Override
            public void onUnableToPlayAd(@NonNull String s, String reason) {
                _logger.info("onUnableToPlayAd: " + reason);
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onUnavailable);
            }

            @Override
            public void onAdAvailabilityUpdate(@NonNull String s, boolean isAdPlayable) {
                _logger.info("onAdAvailabilityUpdate: " + isAdPlayable);
                Utils.checkMainThread();
            }
        });

        vunglePub.loadAd(placementId);

        _initialized = true;
    }


    private boolean hasRewardedVideo(String placementId) {
        Utils.checkMainThread();
        return vunglePub.isAdPlayable(placementId);
    }

    private void showRewardedVideo(String placementId) {
        Utils.checkMainThread();

        vunglePub.playAd(placementId, globalAdConfig);
    }
}
