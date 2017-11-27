package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.Gravity;
import android.widget.FrameLayout;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.internal.Metrics;
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

    private Context      _context;
    private Activity     _activity;
    private AdView       _adView;
    private boolean      _isAdLoaded;
    private String       _adId;
    private AdSize       _adSize;
    private AdViewHelper _helper;
    private boolean      _customSize;

    FacebookBannerAd(@NonNull Context context, @Nullable Activity activity, @NonNull String adId,
                     @NonNull AdSize adSize) {
        Utils.checkMainThread();
        _isAdLoaded = false;
        _adId = adId;
        _adSize = adSize;
        _adView = null;
        _context = context;
        _activity = activity;
        _customSize = false;
        _helper = new AdViewHelper("FacebookBannerAd", _adId);
        createInternalAd();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onDestroy(@NonNull Activity activity) {
        assert _activity == activity;
        removeFromActivity(activity);
        _activity = null;
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _helper = null;
        _context = null;
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
        _customSize = false;
        _isAdLoaded = false;
        AdView adView = new AdView(_context, _adId, _adSize);
        adView.setAdListener(this);
        _adView = adView;

        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        _adView.setLayoutParams(params);

        if (_activity != null) {
            addToActivity(_activity);
        }
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_adView == null) {
            return false;
        }
        _customSize = false;
        _isAdLoaded = false;
        if (_activity != null) {
            removeFromActivity(_activity);
        }
        _adView.destroy();
        _adView = null;
        return true;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_adView);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_adView);
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
        if (_customSize) {
            return AdViewHelper.getSize(_adView);
        }
        int width = getWidthInPixels(_adSize);
        int height = getHeightInPixels(_adSize);
        return new Point(width, height);
    }

    @Override
    public void setSize(@NonNull Point size) {
        AdViewHelper.setSize(size, _adView);
        _customSize = true;
    }

    @Override
    public void setVisible(boolean visible) {
        AdViewHelper.setVisible(visible, _adView);
        if (visible) {
            _adView.setBackgroundColor(Color.BLACK);
        }
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

    private static int getWidthInPixels(AdSize size) {
        switch (size.getWidth()) {
        case 0: // Interstitial.
        case -1: // Normal ads.
            return Resources.getSystem().getDisplayMetrics().widthPixels;
        default: // Deprecated ads.
            return (int) Metrics.convertDpToPixel((double) size.getWidth());
        }
    }

    private static int getHeightInPixels(AdSize size) {
        switch (size.getHeight()) {
        case 0: // Interstitial.
            return Resources.getSystem().getDisplayMetrics().heightPixels;
        default: // Normal ads.
            return (int) Metrics.convertDpToPixel((double) size.getHeight());
        }
    }
}
