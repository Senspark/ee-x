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
import com.vungle.warren.AdConfig;
import com.vungle.warren.InitCallback;
import com.vungle.warren.LoadAdCallback;
import com.vungle.warren.PlayAdCallback;
import com.vungle.warren.error.VungleException;

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
    private static final String k__loadVideoAd       = "Vungle_loadVideoAd";

    private static final Logger _logger = new Logger(Vungle.class.getName());

    private Context _context;
    private boolean _initializing;

    private PlayAdCallback _playAdCallback;
    private LoadAdCallback _loadAdCallback;
    private InitCallback _initCallback;


    public Vungle(Context context) {
        Utils.checkMainThread();
        _logger.debug("constructor begin: context = " + context);
        _context = context;
        _initializing = false;
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
        //do nothing
    }

    @Override
    public void onStart() {
        //do nothing
    }

    @Override
    public void onStop() {
        //do nothing
    }

    @Override
    public void onResume() {
        //do nothing
    }

    @Override
    public void onPause() {
        //do nothing
    }

    @Override
    public void onDestroy() {
        //do nothing
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
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

                initialize(gameId);

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

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                loadVideoAd(message);

                return "";
            }
        }, k__loadVideoAd);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__hasRewardedVideo);
        bridge.deregisterHandler(k__showRewardedVideo);
        bridge.deregisterHandler(k__loadVideoAd);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(final @NonNull String gameId){
        Utils.checkMainThread();
        if (_initializing) {
            return;
        }

        _initializing = true;

        _initCallback = new InitCallback() {
            @Override
            public void onSuccess() {
                _logger.info("vunglePub.init onSuccess");
                _initializing = false;
            }

            @Override
            public void onError(Throwable throwable) {
                _logger.info("vunglePub.init onFailure");
                _initializing = false;
            }

            @Override
            public void onAutoCacheAdAvailable(String s) {

            }
        };

        com.vungle.warren.Vungle.init(gameId, _context, _initCallback);

        _playAdCallback = new PlayAdCallback() {
            @Override
            public void onAdStart(String s) {
                _logger.info("onAdStart");
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onStart);
            }

            @Override
            public void onAdEnd(String s, boolean completed, boolean isCTAClicked
            ) {
                _logger.info("onAdEnd: successful = " + completed + " clicked = " +
                        isCTAClicked);
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onEnd, Utils.toString(completed));
            }

            @Override
            public void onError(String placementReferenceId, Throwable throwable) {
                _logger.info("onUnableToPlayAd: " + placementReferenceId);
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onUnavailable);

                try {
                    VungleException ex = (VungleException) throwable;

                    if (ex.getExceptionCode() == VungleException.VUNGLE_NOT_INTIALIZED) {
                        com.vungle.warren.Vungle.init(gameId, _context, _initCallback);
                    }
                } catch (ClassCastException cex) {
                    _logger.info("Error: " + cex.getMessage());
                }
            }
        };

        _loadAdCallback = new LoadAdCallback() {
            @Override
            public void onAdLoad(String placementReferenceId) {
                _logger.info("onAdLoaded: " + placementReferenceId);
            }

            @Override
            public void onError(String placementReferenceId, Throwable throwable) {
                _logger.info("onAdLoadError: " + placementReferenceId + " reason: " + throwable.getMessage());
            }
        };
    }

    private boolean hasRewardedVideo(String placementId) {
        if (!com.vungle.warren.Vungle.isInitialized()) {
            return false;
        }
        Utils.checkMainThread();
        return com.vungle.warren.Vungle.canPlayAd(placementId);
    }

    private void showRewardedVideo(String placementId) {
        Utils.checkMainThread();
        com.vungle.warren.Vungle.playAd(placementId, new AdConfig(), _playAdCallback);
    }

    private void loadVideoAd(String placementId) {
        Utils.checkMainThread();
        com.vungle.warren.Vungle.loadAd(placementId, _loadAdCallback);
    }
}
