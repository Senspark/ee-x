package com.ee.admob;

import android.content.Context;

import androidx.annotation.NonNull;

import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.IInterstitialAd;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

/**
 * Created by Zinge on 10/13/17.
 */

class AdMobInterstitialAd extends AdListener implements IInterstitialAd {
    private static final Logger _logger = new Logger(AdMobInterstitialAd.class.getName());

    private Context              _context;
    private InterstitialAd       _interstitialAd;
    private String               _adId;
    private InterstitialAdHelper _helper;
    private IMessageBridge       _bridge;


    AdMobInterstitialAd(@NonNull Context context, @NonNull String adId) {
        Utils.checkMainThread();
        _context = context;
        _adId = adId;
        _interstitialAd = null;
        _helper = new InterstitialAdHelper("AdMobInterstitialAd", adId);
        _bridge = MessageBridge.getInstance();
        registerHandlers();
    }

    void destroy() {
        _logger.info("destroy: id = " + _adId);
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _helper = null;
        _context = null;
        _adId = null;
        _helper = null;
        _bridge = null;
    }

    private String kCreateInternalAd() {
        return "AdMobInterstitialAd_createInternalAd_" + _adId;
    }

    private String kDestroyInternalAd() {
        return "AdMobInterstitialAd_destroyInternalAd_" + _adId;
    }

    private String kOnLoaded() {
        return "AdMobInterstitialAd_onLoaded_" + _adId;
    }

    private String kOnFailedToLoad() {
        return "AdMobInterstitialAd_onFailedToLoad_" + _adId;
    }

    private String kOnClosed() {
        return "AdMobInterstitialAd_onClosed_" + _adId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers(this);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, kCreateInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, kDestroyInternalAd());
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();

        _bridge.deregisterHandler(kCreateInternalAd());
        _bridge.deregisterHandler(kDestroyInternalAd());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_interstitialAd != null) {
            return false;
        }
        InterstitialAd interstitialAd = new InterstitialAd(_context);
        interstitialAd.setAdUnitId(_adId);
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
        _interstitialAd = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _interstitialAd != null && _interstitialAd.isLoaded();
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_interstitialAd == null) {
            return;
        }
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        _interstitialAd.loadAd(builder.build());
    }

    @Override
    public boolean show() {
        if (_interstitialAd == null) {
            return false;
        }
        _interstitialAd.show();
        return true;
    }

    @Override
    public void onAdClosed() {
        _logger.info("onAdClosed");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClosed());
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();
        _bridge.callCpp(kOnFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Utils.checkMainThread();
    }

    @Override
    public void onAdOpened() {
        _logger.info("onAdOpened");
        Utils.checkMainThread();
    }

    @Override
    public void onAdLoaded() {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        _bridge.callCpp(kOnLoaded());
    }

    @Override
    public void onAdClicked() {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
    }

    @Override
    public void onAdImpression() {
        _logger.info("onAdImpression");
        Utils.checkMainThread();
    }
}
