package com.ee.facebook;

import android.app.Activity;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

/**
 * Created by Zinge on 10/11/17.
 */

class FacebookInterstitialAd implements InterstitialAdListener {
    private static final Logger _logger = new Logger(FacebookInterstitialAd.class.getName());

    private Activity       _activity;
    private InterstitialAd _interstitialAd;
    private String         _placementId;

    public FacebookInterstitialAd(@NonNull Activity activity, @NonNull String placementId) {
        _activity = activity;
        _placementId = placementId;
        _interstitialAd = null;
        registerHandlers();
    }

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        deregisterHandlers();
        _activity = null;
        _placementId = null;
        _interstitialAd = null;
    }

    private String k__createInternalAd() {
        return "FacebookInterstitialAd_createInternalAd_" + _placementId;
    }

    private String k__destroyInternalAd() {
        return "FacebookInterstitialAd_destroyInternalAd_" + _placementId;
    }

    private String k__isLoaded() {
        return "FacebookInterstitialAd_isLoaded_" + _placementId;
    }

    private String k__load() {
        return "FacebookInterstitialAd_load_" + _placementId;
    }

    private String k__show() {
        return "FacebookInterstitialAd_show_" + _placementId;
    }

    private String k__onLoaded() {
        return "FacebookInterstitialAd_onLoaded_" + _placementId;
    }

    private String k__onFailedToLoad() {
        return "FacebookInterstitialAd_onFailedToLoad_" + _placementId;
    }

    private String k__onClosed() {
        return "FacebookInterstitialAd_onClosed_" + _placementId;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return createInternalAd() ? "true" : "false";
            }
        }, k__createInternalAd());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return destroyInternalAd() ? "true" : "false";
            }
        }, k__destroyInternalAd());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return isLoaded() ? "true" : "false";
            }
        }, k__isLoaded());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                load();
                return "";
            }
        }, k__load());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                show();
                return "";
            }
        }, k__show());
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__createInternalAd());
        bridge.deregisterHandler(k__destroyInternalAd());
        bridge.deregisterHandler(k__isLoaded());
        bridge.deregisterHandler(k__load());
        bridge.deregisterHandler(k__show());
    }

    private boolean createInternalAd() {
        if (_interstitialAd != null) {
            return false;
        }
        InterstitialAd interstitialAd = new InterstitialAd(_activity, _placementId);
        interstitialAd.setAdListener(this);
        _interstitialAd = interstitialAd;
        return true;
    }

    private boolean destroyInternalAd() {
        if (_interstitialAd == null) {
            return false;
        }
        _interstitialAd.setAdListener(null);
        _interstitialAd.destroy();
        _interstitialAd = null;
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isLoaded() {
        return _interstitialAd != null && _interstitialAd.isAdLoaded();
    }

    @SuppressWarnings("WeakerAccess")
    public void load() {
        if (_interstitialAd == null) {
            return;
        }
        _interstitialAd.loadAd();
    }

    @SuppressWarnings("WeakerAccess")
    public void show() {
        if (_interstitialAd == null) {
            return;
        }
        _interstitialAd.show();
    }

    @Override
    public void onInterstitialDisplayed(Ad ad) {
        _logger.info("onInterstitialDisplayed");
    }

    @Override
    public void onInterstitialDismissed(Ad ad) {
        _logger.info("onInterstitialDismissed");
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onClosed());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
    }
}
