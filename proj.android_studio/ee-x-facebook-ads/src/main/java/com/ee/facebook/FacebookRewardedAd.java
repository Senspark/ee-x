package com.ee.facebook;

import android.content.Context;

import androidx.annotation.NonNull;

import com.ee.ads.MessageHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.RewardedVideoAd;
import com.facebook.ads.RewardedVideoAdListener;

/**
 * Created by Zinge on 10/11/17.
 */

class FacebookRewardedAd implements RewardedVideoAdListener {
    private static final Logger _logger = new Logger(FacebookRewardedAd.class.getName());

    private Context _context;
    private IMessageBridge _bridge;
    private String _adId;
    private MessageHelper _messageHelper;
    private RewardedVideoAd _ad;
    private boolean _rewarded;

    FacebookRewardedAd(@NonNull Context context, @NonNull String adId) {
        _logger.info("constructor: adId = %s", adId);
        Utils.checkMainThread();
        _context = context;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _messageHelper = new MessageHelper("FacebookRewardedAd", adId);
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        _logger.info("destroy: adId = %s", _adId);
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _messageHelper = null;
    }

    private void registerHandlers() {
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

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isLoaded());
            }
        }, _messageHelper.isLoaded());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                load();
                return "";
            }
        }, _messageHelper.load());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                show();
                return "";
            }
        }, _messageHelper.show());
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(_messageHelper.createInternalAd());
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd());
        _bridge.deregisterHandler(_messageHelper.isLoaded());
        _bridge.deregisterHandler(_messageHelper.load());
        _bridge.deregisterHandler(_messageHelper.show());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _ad = new RewardedVideoAd(_context, _adId);
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

    private boolean isLoaded() {
        Utils.checkMainThread();
        return _ad.isAdLoaded();
    }

    private void load() {
        _logger.info("load");
        Utils.checkMainThread();
        _ad.loadAd(_ad.buildLoadAdConfig().withAdListener(this).build());
    }

    private void show() {
        _logger.info("show");
        Utils.checkMainThread();
        _rewarded = false;
        boolean result = _ad.show(_ad.buildShowAdConfig().build());
        if (result) {
            // OK.
        } else {
            _bridge.callCpp(_messageHelper.onFailedToShow());
        }
    }

    @Override
    public void onRewardedVideoCompleted() {
        _logger.info("onRewardedVideoCompleted");
        _rewarded = true;
    }

    @Override
    public void onRewardedVideoClosed() {
        _logger.info("onRewardedVideoClosed");
        _bridge.callCpp(_messageHelper.onClosed(), Utils.toString(_rewarded));
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        _bridge.callCpp(_messageHelper.onClicked());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError");
        _bridge.callCpp(_messageHelper.onFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        _bridge.callCpp(_messageHelper.onLoaded());
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
    }
}
