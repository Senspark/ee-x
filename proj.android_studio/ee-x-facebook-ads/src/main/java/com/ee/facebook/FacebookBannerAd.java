package com.ee.facebook;

import android.app.Activity;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
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

    public static AdSize adSizeFor(int index) {
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

    private Activity _activity;
    private AdView   _adView;
    private boolean  _isAdLoaded;
    private String   _adId;
    private AdSize   _adSize;

    public FacebookBannerAd(@NonNull Activity activity, @NonNull String adId,
                            @NonNull AdSize adSize) {
        _isAdLoaded = false;
        _adId = adId;
        _adSize = adSize;
        _adView = null;
        _activity = activity;
        createInternalAd();
        registerHandlers();
    }

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        deregisterHandlers();
        destroyInternalAd();
        _activity = null;
        _adId = null;
        _adSize = null;
    }

    private void registerHandlers() {
        AdViewHelper helper = new AdViewHelper("FacebookBannerAd", _adId);
        helper.registerHandlers(this);
    }

    private void deregisterHandlers() {
        AdViewHelper helper = new AdViewHelper("FacebookBannerAd", _adId);
        helper.deregisterHandlers();
    }

    private boolean createInternalAd() {
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
        if (_adView == null) {
            return false;
        }
        _adView.destroy();
        _adView = null;
        return true;
    }

    @Override
    public boolean isLoaded() {
        return _adView != null && _isAdLoaded;
    }

    @Override
    public void load() {
        if (_adView == null) {
            return;
        }
        _adView.loadAd();
    }

    @Override
    @NonNull
    public Point getPosition() {
        int p[] = new int[2];
        _adView.getLocationInWindow(p);
        return new Point(p[0], p[1]);
    }

    @Override
    public void setPosition(@NonNull Point position) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _adView.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        _adView.setLayoutParams(params);
    }

    @Override
    @NonNull
    public Point getSize() {
        return new Point(_adView.getWidth(), _adView.getHeight());
    }

    @Override
    public void setSize(@NonNull Point size) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _adView.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        _adView.setLayoutParams(params);
    }

    @Override
    public void setVisible(boolean visible) {
        if (visible) {
            _adView.setVisibility(View.VISIBLE);
        } else {
            _adView.setVisibility(View.INVISIBLE);
        }
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onAdLoaded: " + adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        _isAdLoaded = true;
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
