package com.ee.admob;

import android.content.Context;
import android.support.annotation.NonNull;

import com.ee.ads.InterstitialAdHelper;
import com.ee.ads.InterstitialAdInterface;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import java.util.List;

/**
 * Created by Zinge on 10/13/17.
 */

class AdMobInterstitialAd extends AdListener implements InterstitialAdInterface {
    private static final Logger _logger = new Logger(AdMobInterstitialAd.class.getName());

    private Context              _context;
    private InterstitialAd       _interstitialAd;
    private String               _adId;
    private List<String>         _testDevices;
    private InterstitialAdHelper _helper;

    AdMobInterstitialAd(@NonNull Context context, @NonNull String adId,
                        @NonNull List<String> testDevices) {
        Utils.checkMainThread();
        _context = context;
        _adId = adId;
        _interstitialAd = null;
        _testDevices = testDevices;
        _helper = new InterstitialAdHelper("AdMobInterstitialAd", adId);
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
        _testDevices = null;
        _helper = null;
    }

    private String k__createInternalAd() {
        return "AdMobInterstitialAd_createInternalAd_" + _adId;
    }

    private String k__destroyInternalAd() {
        return "AdMobInterstitialAd_destroyInternalAd_" + _adId;
    }

    private String k__onLoaded() {
        return "AdMobInterstitialAd_onLoaded_" + _adId;
    }

    private String k__onFailedToLoad() {
        return "AdMobInterstitialAd_onFailedToLoad_" + _adId;
    }

    private String k__onClosed() {
        return "AdMobInterstitialAd_onClosed_" + _adId;
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
        for (String hash : _testDevices) {
            builder.addTestDevice(hash);
        }
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

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onClosed());
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad(), String.valueOf(errorCode));
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

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onLoaded());
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
