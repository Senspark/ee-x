package com.ee.admob;

import android.app.Activity;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.Gravity;
import android.widget.FrameLayout;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

import java.util.List;

/**
 * Created by Zinge on 10/13/17.
 */

class AdMobBannerAd extends AdListener implements AdViewInterface {
    private static final Logger _logger = new Logger(AdMobBannerAd.class.getName());

    static AdSize adSizeFor(int index) {
        if (index == 0) {
            return AdSize.BANNER;
        }
        if (index == 1) {
            return AdSize.LARGE_BANNER;
        }
        if (index == 2) {
            return AdSize.SMART_BANNER;
        }
        return AdSize.BANNER;
    }

    private Activity     _activity;
    private AdView       _adView;
    private boolean      _isAdLoaded;
    private String       _adId;
    private AdSize       _adSize;
    private List<String> _testDevices;
    private AdViewHelper _helper;

    AdMobBannerAd(@NonNull Activity activity, @NonNull String adId, @NonNull AdSize adSize,
                  @NonNull List<String> testDevices) {
        Utils.checkMainThread();
        _isAdLoaded = false;
        _activity = activity;
        _adId = adId;
        _adSize = adSize;
        _adView = null;
        _testDevices = testDevices;
        _helper = new AdViewHelper("AdMobBannerAd", _adId);
        createInternalAd();
        registerHandlers();
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _activity = null;
        _adId = null;
        _adSize = null;
        _testDevices = null;
        _helper = null;
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
        AdView adView = new AdView(_activity);
        adView.setAdSize(_adSize);
        adView.setAdListener(this);
        adView.setAdUnitId(_adId);
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
        AdRequest.Builder builder = new AdRequest.Builder();
        for (String hash : _testDevices) {
            builder.addTestDevice(hash);
        }
        _adView.loadAd(builder.build());
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
    public void onAdClosed() {
        _logger.info("onAdClosed");
        Utils.checkMainThread();
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();
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
        _isAdLoaded = true;
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
