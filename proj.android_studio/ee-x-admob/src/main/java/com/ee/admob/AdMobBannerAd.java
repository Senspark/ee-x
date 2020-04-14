package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Point;
import android.view.Gravity;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

/**
 * Created by Zinge on 10/13/17.
 */

class AdMobBannerAd extends AdListener implements IAdView {
    private static final Logger _logger = new Logger(AdMobBannerAd.class.getName());

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private AdView _adView;
    private boolean _isAdLoaded;
    private String _adId;
    private AdSize _adSize;
    private AdViewHelper _helper;
    private ViewHelper _viewHelper;
    private boolean _customSize;

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
        if (index == 3) {
            return AdSize.MEDIUM_RECTANGLE;
        }
        return AdSize.BANNER;
    }

    AdMobBannerAd(@NonNull Context context, @Nullable Activity activity, @NonNull String adId, @NonNull AdSize adSize) {
        Utils.checkMainThread();
        _isAdLoaded = false;
        _context = context;
        _activity = activity;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _adSize = adSize;
        _adView = null;
        _customSize = false;
        _helper = new AdViewHelper(_bridge, this, "AdMobBannerAd", _adId);
        _viewHelper = null;

        createInternalAd();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onResume() {
        Utils.checkMainThread();
        _adView.resume();
    }

    void onPause() {
        Utils.checkMainThread();
        _adView.pause();
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
        _context = null;
        _adId = null;
        _adSize = null;
        _helper = null;
        _bridge = null;
    }

    @NonNull
    private String kOnLoaded() {
        return "AdMobBannerAd_onLoaded_" + _adId;
    }

    @NonNull
    private String kOnFailedToLoad() {
        return "AdMobBannerAd_onFailedToLoad_" + _adId;
    }

    @NonNull
    private String kOnClicked() {
        return "AdMobBannerAd_onClicked_" + _adId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers();
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
        AdView adView = new AdView(_context);
        adView.setAdSize(_adSize);
        adView.setAdListener(this);
        adView.setAdUnitId(_adId);
        _adView = adView;

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        _adView.setLayoutParams(params);
        _viewHelper = new ViewHelper(_adView);

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
        _viewHelper = null;
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
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        _adView.loadAd(builder.build());
    }

    @NonNull
    @Override
    public Point getPosition() {
        return _viewHelper.getPosition();
    }

    @Override
    public void setPosition(@NonNull Point position) {
        _viewHelper.setPosition(position);
    }

    @NonNull
    @Override
    public Point getSize() {
        if (_customSize) {
            return _viewHelper.getSize();
        }
        int width = _adSize.getWidthInPixels(_context);
        int height = _adSize.getHeightInPixels(_context);
        return new Point(width, height);
    }

    @Override
    public void setSize(@NonNull Point size) {
        _viewHelper.setSize(size);
        _customSize = true;
    }

    @Override
    public boolean isVisible() {
        return _viewHelper.isVisible();
    }

    @Override
    public void setVisible(boolean visible) {
        _viewHelper.setVisible(visible);
        if (visible) {
            // https://stackoverflow.com/questions/21408178/admob-wont-show-the-banner-until
            // -refresh-or-sign-in-to-google-plus
            _adView.setBackgroundColor(Color.BLACK);
        }
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
        _bridge.callCpp(kOnFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClicked());
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
