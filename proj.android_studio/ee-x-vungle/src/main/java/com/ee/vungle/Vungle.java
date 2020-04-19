package com.ee.vungle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.vungle.warren.AdConfig;
import com.vungle.warren.InitCallback;
import com.vungle.warren.LoadAdCallback;
import com.vungle.warren.PlayAdCallback;

import java.util.HashMap;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Vungle implements PluginProtocol {
    private static final String kPrefix = "Vungle";
    private static final String k__initialize = kPrefix + "_initialize";
    private static final String k__hasRewardedAd = kPrefix + "_hasRewardedAd";
    private static final String k__loadRewardedAd = kPrefix + "_loadRewardedAd";
    private static final String k__showRewardedAd = kPrefix + "_showRewardedAd";
    private static final String k__onLoaded = kPrefix + "_onLoaded";
    private static final String k__onFailedToLoad = kPrefix + "_onFailedToLoad";
    private static final String k__onFailedToShow = kPrefix + "_onFailedToShow";
    private static final String k__onClosed = kPrefix + "_onClosed";

    private static final Logger _logger = new Logger(Vungle.class.getName());

    private Context _context;
    private IMessageBridge _bridge;
    private boolean _initializing;

    private PlayAdCallback _playAdCallback;
    private LoadAdCallback _loadAdCallback;
    private InitCallback _initCallback;

    public Vungle(Context context) {
        Utils.checkMainThread();
        _logger.debug("constructor begin: context = " + context);
        _context = context;
        _bridge = MessageBridge.getInstance();
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

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assertThat(dict).isNotNull();
                String gameId = (String) dict.get("gameId");
                initialize(gameId);
                return "";
            }
        }, k__initialize);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(hasRewardedAd(message));
            }
        }, k__hasRewardedAd);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                loadRewardedAd(message);
                return "";
            }
        }, k__loadRewardedAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showRewardedAd(message);
                return "";
            }
        }, k__showRewardedAd);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _bridge.deregisterHandler(k__initialize);
        _bridge.deregisterHandler(k__hasRewardedAd);
        _bridge.deregisterHandler(k__showRewardedAd);
        _bridge.deregisterHandler(k__loadRewardedAd);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(final @NonNull String gameId) {
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
            public void onAutoCacheAdAvailable(String adId) {
            }
        };

        com.vungle.warren.Vungle.init(gameId, _context, _initCallback);

        _playAdCallback = new PlayAdCallback() {
            @Override
            public void onAdStart(String adId) {
                _logger.info("onAdStart");
            }

            @Override
            public void onAdEnd(String adId, boolean completed, boolean isCTAClicked) {
                _logger.info("onAdEnd: successful = " + completed + " clicked = " + isCTAClicked);
                Map<String, Object> dict = new HashMap<>();
                dict.put("ad_id", adId);
                dict.put("rewarded", completed);
                _bridge.callCpp(k__onClosed, JsonUtils.convertDictionaryToString(dict));
            }

            @Override
            public void onError(String adId, Throwable throwable) {
                _logger.info("onUnableToPlayAd: " + adId);
                Map<String, Object> dict = new HashMap<>();
                dict.put("ad_id", adId);
                dict.put("message", throwable.getMessage());
                _bridge.callCpp(k__onFailedToShow, JsonUtils.convertDictionaryToString(dict));
            }
        };

        _loadAdCallback = new LoadAdCallback() {
            @Override
            public void onAdLoad(String adId) {
                _logger.info("onAdLoaded: " + adId);
                Map<String, Object> dict = new HashMap<>();
                dict.put("ad_id", adId);
                _bridge.callCpp(k__onLoaded, JsonUtils.convertDictionaryToString(dict));
            }

            @Override
            public void onError(String adId, Throwable throwable) {
                _logger.info("onAdLoadError: " + adId + " reason: " + throwable.getMessage());
                Map<String, Object> dict = new HashMap<>();
                dict.put("ad_id", adId);
                dict.put("message", throwable.getMessage());
                _bridge.callCpp(k__onFailedToLoad, JsonUtils.convertDictionaryToString(dict));
            }
        };
    }

    private boolean hasRewardedAd(String adId) {
        if (!com.vungle.warren.Vungle.isInitialized()) {
            return false;
        }
        Utils.checkMainThread();
        return com.vungle.warren.Vungle.canPlayAd(adId);
    }

    private void loadRewardedAd(String adId) {
        Utils.checkMainThread();
        com.vungle.warren.Vungle.loadAd(adId, _loadAdCallback);
    }

    private void showRewardedAd(String adId) {
        Utils.checkMainThread();
        com.vungle.warren.Vungle.playAd(adId, new AdConfig(), _playAdCallback);
    }
}
