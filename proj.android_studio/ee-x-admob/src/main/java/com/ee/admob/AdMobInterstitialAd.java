package com.ee.admob;

import android.content.Context;

import androidx.annotation.NonNull;

import com.ee.ads.IInterstitialAd;
import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.MessageHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.internal.Thread;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/13/17.
 */

class AdMobInterstitialAd extends AdListener implements IInterstitialAd {
    private static final Logger _logger = new Logger(AdMobInterstitialAd.class.getName());

    private Context _context;
    private IMessageBridge _bridge;
    private String _adId;
    private MessageHelper _messageHelper;
    private InterstitialAdHelper _helper;
    private InterstitialAd _ad;

    AdMobInterstitialAd(@NonNull Context context,
                        @NonNull IMessageBridge bridge,
                        @NonNull String adId) {
        _logger.info("constructor: adId = %s", adId);
        Thread.checkMainThread();
        _context = context;
        _bridge = bridge;
        _adId = adId;
        _messageHelper = new MessageHelper("AdMobInterstitialAd", adId);
        _helper = new InterstitialAdHelper(_bridge, this, _messageHelper);
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        _logger.info("destroy: adId = " + _adId);
        Thread.checkMainThread();
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

        _bridge.registerHandler(_messageHelper.createInternalAd(), message ->
            Utils.toString(createInternalAd()));

        _bridge.registerHandler(_messageHelper.destroyInternalAd(), message ->
            Utils.toString(destroyInternalAd()));
    }

    private void deregisterHandlers() {
        _helper.deregisterHandlers();
        _bridge.deregisterHandler(_messageHelper.createInternalAd());
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd());
    }

    private boolean createInternalAd() {
        Thread.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _ad = new InterstitialAd(_context);
        _ad.setAdUnitId(_adId);
        _ad.setAdListener(this);
        return true;
    }

    private boolean destroyInternalAd() {
        Thread.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _ad.setAdListener(null);
        _ad = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        return _ad.isLoaded();
    }

    @Override
    public void load() {
        Thread.checkMainThread();
        _logger.info("load");
        assertThat(_ad).isNotNull();
        AdRequest.Builder builder = new AdRequest.Builder();
        _ad.loadAd(builder.build());
    }

    @Override
    public void show() {
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        _ad.show();
    }

    @Override
    public void onAdClosed() {
        _logger.info("onAdClosed");
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onClosed());
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onClicked());
    }

    @Override
    public void onAdOpened() {
        _logger.info("onAdOpened");
        Thread.checkMainThread();
    }

    @Override
    public void onAdLoaded() {
        _logger.info("onAdLoaded");
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onLoaded());
    }

    @Override
    public void onAdClicked() {
        _logger.info("onAdClicked");
        Thread.checkMainThread();
        // https://stackoverflow.com/questions/47814295/interstitialad-listener-onadclicked-not-working
        // Use onAdLeftApplication instead.
    }

    @Override
    public void onAdImpression() {
        _logger.info("onAdImpression");
        Thread.checkMainThread();
    }
}
