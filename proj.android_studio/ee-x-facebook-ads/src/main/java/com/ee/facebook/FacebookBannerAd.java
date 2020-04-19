package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Point;
import android.view.Gravity;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.ads.MessageHelper;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.AdSize;
import com.facebook.ads.AdView;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookBannerAd implements AdListener, IAdView {
    private static final Logger _logger = new Logger(FacebookBannerAd.class.getName());

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private String _adId;
    private AdSize _adSize;
    private MessageHelper _messageHelper;
    private AdViewHelper _helper;
    private ViewHelper _viewHelper;
    private boolean _customSize;
    private boolean _isLoaded;
    private AdView _ad;

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

    FacebookBannerAd(@NonNull Context context, @Nullable Activity activity, @NonNull String adId, @NonNull AdSize adSize) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _adSize = adSize;
        _messageHelper = new MessageHelper("FacebookBannerAd", adId);
        _helper = new AdViewHelper(_bridge, this, _messageHelper);

        createInternalAd();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onDestroy(@NonNull Activity activity) {
        assertThat(_activity).isEqualTo(activity);
        removeFromActivity(activity);
        _activity = null;
    }

    void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _adSize = null;
        _messageHelper = null;
        _helper = null;
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
        if (_ad != null) {
            return false;
        }
        _customSize = false;
        _isLoaded = false;
        _ad = new AdView(_context, _adId, _adSize);

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        _ad.setLayoutParams(params);
        _viewHelper = new ViewHelper(_ad);

        if (_activity != null) {
            addToActivity(_activity);
        }
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _customSize = false;
        _isLoaded = false;
        if (_activity != null) {
            removeFromActivity(_activity);
        }
        _viewHelper = null;
        _ad.destroy();
        _ad = null;
        return true;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_ad);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_ad);
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        return _isLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        _ad.loadAd(_ad.buildLoadAdConfig().withAdListener(this).build());
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
        int width = getWidthInPixels(_adSize);
        int height = getHeightInPixels(_adSize);
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
            _ad.setBackgroundColor(Color.BLACK);
        }
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
        _isLoaded = true;
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

    private static int getWidthInPixels(AdSize size) {
        switch (size.getWidth()) {
            case 0: // Interstitial.
            case -1: // Normal ads.
                return Resources.getSystem().getDisplayMetrics().widthPixels;
            default: // Deprecated ads.
                return (int) Utils.convertDpToPixel((double) size.getWidth());
        }
    }

    private static int getHeightInPixels(AdSize size) {
        switch (size.getHeight()) {
            case 0: // Interstitial.
                return Resources.getSystem().getDisplayMetrics().heightPixels;
            default: // Normal ads.
                return (int) Utils.convertDpToPixel((double) size.getHeight());
        }
    }
}
