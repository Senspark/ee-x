package com.ee.facebook;

import android.app.Activity;
import android.support.annotation.NonNull;

import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.InterstitialAdInterface;
import com.ee.core.Logger;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

/**
 * Created by Zinge on 10/11/17.
 */

class FacebookInterstitialAd implements InterstitialAdListener, InterstitialAdInterface {
    private static final Logger _logger = new Logger(FacebookInterstitialAd.class.getName());

    private Activity             _activity;
    private InterstitialAd       _interstitialAd;
    private String               _placementId;
    private InterstitialAdHelper _helper;

    public FacebookInterstitialAd(@NonNull Activity activity, @NonNull String placementId) {
        Utils.checkMainThread();
        _activity = activity;
        _placementId = placementId;
        _interstitialAd = null;
        _helper = new InterstitialAdHelper("FacebookInterstitialAd", placementId);
        registerHandlers();
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _helper = null;
        _activity = null;
        _placementId = null;
    }

    private String k__createInternalAd() {
        return "FacebookInterstitialAd_createInternalAd_" + _placementId;
    }

    private String k__destroyInternalAd() {
        return "FacebookInterstitialAd_destroyInternalAd_" + _placementId;
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
        Utils.checkMainThread();
        _helper.registerHandlers(this);

        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, k__createInternalAd());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, k__destroyInternalAd());
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();

        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__createInternalAd());
        bridge.deregisterHandler(k__destroyInternalAd());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_interstitialAd != null) {
            return false;
        }
        InterstitialAd interstitialAd = new InterstitialAd(_activity, _placementId);
        interstitialAd.setAdListener(this);
        _interstitialAd = interstitialAd;
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_interstitialAd == null) {
            return false;
        }
        _interstitialAd.setAdListener(null);
        _interstitialAd.destroy();
        _interstitialAd = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _interstitialAd != null && _interstitialAd.isAdLoaded();
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_interstitialAd == null) {
            return;
        }
        _interstitialAd.loadAd();
    }

    @Override
    public boolean show() {
        Utils.checkMainThread();
        return _interstitialAd != null && _interstitialAd.show();
    }

    @Override
    public void onInterstitialDisplayed(Ad ad) {
        _logger.info("onInterstitialDisplayed");
        Utils.checkMainThread();
    }

    @Override
    public void onInterstitialDismissed(Ad ad) {
        _logger.info("onInterstitialDismissed");
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onClosed());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
        Utils.checkMainThread();
    }
}
