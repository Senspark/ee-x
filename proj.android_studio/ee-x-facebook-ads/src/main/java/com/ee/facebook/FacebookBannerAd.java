package com.ee.facebook;

import android.app.Activity;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.Gravity;
import android.widget.FrameLayout;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.AdSize;
import com.facebook.ads.AdView;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookBannerAd implements AdListener, AdViewInterface {
    private static final Logger _logger = new Logger(FacebookBannerAd.class.getName());

    static AdSize adSizeFor(int index) {
        if (index == 0) {
            return AdSize.BANNER_HEIGHT_50;
        }
        if (index == 1) {
            return AdSize.BANNER_HEIGHT_90;
        }
        if (index == 2) {
            return AdSize.INTERSTITIAL;
        }
        if (index == 3) {
            return AdSize.RECTANGLE_HEIGHT_250;
        }
        return AdSize.BANNER_320_50;
    }

    private Activity     _activity;
    private AdView       _adView;
    private boolean      _isAdLoaded;
    private String       _adId;
    private AdSize       _adSize;
    private AdViewHelper _helper;

    FacebookBannerAd(@NonNull Activity activity, @NonNull String adId, @NonNull AdSize adSize) {
        Utils.checkMainThread();
        _isAdLoaded = false;
        _adId = adId;
        _adSize = adSize;
        _adView = null;
        _activity = activity;
        _helper = new AdViewHelper("FacebookBannerAd", _adId);
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _helper = null;
        _activity = null;
        _adId = null;
        _adSize = null;
    }

    @NonNull
    private String k__onLoaded() {
        return "FacebookBannerAd_onLoaded_" + _adId;
    }

    @NonNull
    private String k__onFailedToLoad() {
        return "FacebookBannerAd_onFailedToLoad_" + _adId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers(this);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_adView != null) {
            return false;
        }
        _isAdLoaded = false;
        AdView adView = new AdView(_activity, _adId, _adSize);
        adView.setAdListener(this);
        _adView = adView;

        FrameLayout rootView = Utils.getRootView(_activity);
        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        rootView.addView(_adView, params);
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_adView == null) {
            return false;
        }
        _isAdLoaded = false;
        _adView.destroy();
        _adView = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _adView != null && _isAdLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_adView == null) {
            return;
        }
        _adView.loadAd();
    }

    @NonNull
    @Override
    public Point getPosition() {
        return AdViewHelper.getPosition(_adView);
    }

    @Override
    public void setPosition(@NonNull Point position) {
        AdViewHelper.setPosition(position, _adView);
    }

    @NonNull
    @Override
    public Point getSize() {
        return AdViewHelper.getSize(_adView);
    }

    @Override
    public void setSize(@NonNull Point size) {
        AdViewHelper.setSize(size, _adView);
    }

    @Override
    public void setVisible(boolean visible) {
        AdViewHelper.setVisible(visible, _adView);
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
        _isAdLoaded = true;

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
