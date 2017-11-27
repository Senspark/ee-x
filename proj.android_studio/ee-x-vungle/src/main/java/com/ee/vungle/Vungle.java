package com.ee.vungle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
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
    private boolean _initializing;

    private final VunglePub _vunglePub = VunglePub.getInstance();
    private AdConfig _globalAdConfig;

    public Vungle(Context context) {
        Utils.checkMainThread();
        _logger.debug("constructor begin: context = " + context);
        _context = context;
        _initializing = false;
        _globalAdConfig = null;
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
        _vunglePub.onResume();
    }

    @Override
    public void onPause() {
        Utils.checkMainThread();
        _vunglePub.onPause();
    }

    @Override
    public void onDestroy() {
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        _vunglePub.clearEventListeners();
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
        if (_initializing) {
            return;
        }

        _initializing = true;
        _vunglePub.init(_context, gameId, new String[] { placementId }, new VungleInitListener() {
            @Override
            public void onSuccess() {
                _logger.info("vunglePub.init onSuccess");
                _globalAdConfig = _vunglePub.getGlobalAdConfig();
                _vunglePub.loadAd(placementId);
                _initializing = false;
            }
            @Override
            public void onFailure(Throwable e) {
                _logger.info("vunglePub.init onFailure");
                _initializing = false;
            }
        });
        _vunglePub.addEventListeners(new VungleAdEventListener() {
            @Override
            public void onAdEnd(@NonNull String s, boolean wasSuccessfulView, boolean wasCallToActionClicked) {
                _logger.info("onAdEnd: successful = " + wasSuccessfulView + " clicked = " +
                             wasCallToActionClicked);
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onEnd, Utils.toString(wasSuccessfulView));
            }

            @Override
            public void onAdStart(@NonNull String s) {
                _logger.info("onAdStart");
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onStart);
            }

            @Override
            public void onUnableToPlayAd(@NonNull String s, String reason) {
                _logger.info("onUnableToPlayAd: " + reason);
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onUnavailable);
            }

            @Override
            public void onAdAvailabilityUpdate(@NonNull String s, boolean isAdPlayable) {
                _logger.info("onAdAvailabilityUpdate: " + isAdPlayable);
            }
        });
    }

    private boolean hasRewardedVideo(String placementId) {
        if (!_vunglePub.isInitialized()) {
            return false;
        }
        Utils.checkMainThread();
        return _vunglePub.isAdPlayable(placementId);
    }

    private void showRewardedVideo(String placementId) {
        Utils.checkMainThread();
        if (!_vunglePub.isInitialized()) {
            return;
        }
        _vunglePub.playAd(placementId, _globalAdConfig);
    }
}
