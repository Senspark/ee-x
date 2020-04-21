package com.ee.facebook;

import android.content.Context;

import androidx.annotation.NonNull;

import com.ee.ads.IInterstitialAd;
import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.MessageHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/11/17.
 */

class FacebookInterstitialAd implements InterstitialAdListener, IInterstitialAd {
    private static final Logger _logger = new Logger(FacebookInterstitialAd.class.getName());

    private Context _context;
    private IMessageBridge _bridge;
    private String _adId;
    private MessageHelper _messageHelper;
    private InterstitialAdHelper _helper;
    private InterstitialAd _ad;

    FacebookInterstitialAd(@NonNull Context context, @NonNull String adId) {
        _logger.info("constructor: adId = %s", adId);
        Utils.checkMainThread();
        _context = context;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _messageHelper = new MessageHelper("FacebookInterstitialAd", adId);
        _helper = new InterstitialAdHelper(_bridge, this, _messageHelper);
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        _logger.info("destroy: adId = " + _adId);
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _messageHelper = null;
        _helper = null;
    }

    private void registerHandlers() {
        _helper.registerHandlers();

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, _messageHelper.createInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, _messageHelper.destroyInternalAd());
    }

    private void deregisterHandlers() {
        _helper.deregisterHandlers();
        _bridge.deregisterHandler(_messageHelper.createInternalAd());
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _ad = new InterstitialAd(_context, _adId);
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _ad.destroy();
        _ad = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        return _ad.isAdLoaded();
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        _logger.info("load");
        _ad.loadAd(_ad.buildLoadAdConfig().withAdListener(this).build());
    }

    @Override
    public void show() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        boolean result = _ad.show(_ad.buildShowAdConfig().build());
        if (result) {
            // OK.
        } else {
            _bridge.callCpp(_messageHelper.onFailedToLoad());
        }
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
        _bridge.callCpp(_messageHelper.onClosed());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onClicked());
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
        Utils.checkMainThread();
    }
}
