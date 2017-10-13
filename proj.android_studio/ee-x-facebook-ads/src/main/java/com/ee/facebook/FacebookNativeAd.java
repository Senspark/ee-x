package com.ee.facebook;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdChoicesView;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.MediaView;
import com.facebook.ads.NativeAd;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookNativeAd implements AdListener, AdViewInterface {
    private static final Logger _logger = new Logger(FacebookNativeAd.class.getName());

    private NativeAd                _nativeAd;
    private View                    _nativeAdView;
    private boolean                 _isAdLoaded;
    private Activity                _activity;
    private FacebookNativeAdBuilder _builder;
    private AdViewHelper            _helper;

    public FacebookNativeAd(@NonNull Activity activity, @NonNull FacebookNativeAdBuilder builder) {
        Utils.checkMainThread();
        _activity = activity;
        _builder = builder;

        _nativeAd = null;
        _nativeAdView = null;
        _isAdLoaded = false;
        _helper = new AdViewHelper("FacebookNativeAd", _builder.adId);

        createInternalAd();
        createView();
        registerHandlers();
    }

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        destroyView();
        _builder = null;
        _activity = null;
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
        if (_nativeAd != null) {
            return false;
        }
        _isAdLoaded = false;
        NativeAd nativeAd = new NativeAd(_activity, _builder.adId);
        nativeAd.setAdListener(this);
        _nativeAd = nativeAd;
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_nativeAd == null) {
            return false;
        }
        _isAdLoaded = false;
        _nativeAd.unregisterView();
        _nativeAd.destroy();
        _nativeAd = null;
        return true;
    }

    private void createView() {
        Utils.checkMainThread();
        FrameLayout rootView = Utils.getRootView(_activity);
        int layoutId = rootView
            .getResources()
            .getIdentifier(_builder.layoutName, "layout", _activity.getPackageName());
        View nativeAdView = LayoutInflater.from(_activity).inflate(layoutId, null);
        nativeAdView.setVisibility(View.INVISIBLE);

        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        rootView.addView(nativeAdView);
        _nativeAdView = nativeAdView;
    }

    private void destroyView() {
        Utils.checkMainThread();
        FrameLayout rootView = Utils.getRootView(_activity);
        rootView.removeView(_nativeAdView);
        _nativeAdView = null;
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _nativeAd != null && _isAdLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_nativeAd == null) {
            return;
        }
        _nativeAd.loadAd();
    }

    @Override
    @NonNull
    public Point getPosition() {
        Utils.checkMainThread();
        int p[] = new int[2];
        _nativeAdView.getLocationInWindow(p);
        return new Point(p[0], p[1]);
    }

    @Override
    public void setPosition(@NonNull Point position) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params =
            (FrameLayout.LayoutParams) _nativeAdView.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        _nativeAdView.setLayoutParams(params);
    }

    @Override
    @NonNull
    public Point getSize() {
        Utils.checkMainThread();
        return new Point(_nativeAdView.getWidth(), _nativeAdView.getHeight());
    }

    @Override
    public void setSize(@NonNull Point size) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params =
            (FrameLayout.LayoutParams) _nativeAdView.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        _nativeAdView.setLayoutParams(params);
    }

    @Override
    public void setVisible(boolean visible) {
        Utils.checkMainThread();
        if (visible) {
            _nativeAdView.setVisibility(View.VISIBLE);
        } else {
            _nativeAdView.setVisibility(View.INVISIBLE);
        }
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onAdLoaded: " + adError.getErrorMessage());
        Utils.checkMainThread();
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();

        _isAdLoaded = true;

        _nativeAd.unregisterView();

        Resources resources = _activity.getResources();
        int iconId = resources.getIdentifier(_builder.icon, "id", _activity.getPackageName());
        int titleId = resources.getIdentifier(_builder.title, "id", _activity.getPackageName());
        int mediaId = resources.getIdentifier(_builder.media, "id", _activity.getPackageName());
        int socialContextId =
            resources.getIdentifier(_builder.socialContext, "id", _activity.getPackageName());
        int adChoicesId =
            resources.getIdentifier(_builder.adChoices, "id", _activity.getPackageName());
        int bodyId = resources.getIdentifier(_builder.body, "id", _activity.getPackageName());
        int actionId = resources.getIdentifier(_builder.action, "id", _activity.getPackageName());

        ImageView iconImage = _nativeAdView.findViewById(iconId);
        TextView titleLabel = _nativeAdView.findViewById(titleId);
        MediaView mediaView = _nativeAdView.findViewById(mediaId);
        TextView socialContextLabel = _nativeAdView.findViewById(socialContextId);
        TextView bodyLabel = _nativeAdView.findViewById(bodyId);
        Button callToActionButton = _nativeAdView.findViewById(actionId);

        NativeAd.downloadAndDisplayImage(_nativeAd.getAdIcon(), iconImage);
        titleLabel.setText(_nativeAd.getAdTitle());
        socialContextLabel.setText(_nativeAd.getAdSocialContext());
        bodyLabel.setText(_nativeAd.getAdBody());
        callToActionButton.setText(_nativeAd.getAdCallToAction());

        if (mediaView != null) {
            mediaView.setNativeAd(_nativeAd);
        }

        LinearLayout adChoicesViews = _nativeAdView.findViewById(adChoicesId);
        adChoicesViews.addView(new AdChoicesView(_activity, _nativeAd, true));


        List<View> clickableViews = new ArrayList<>();
        clickableViews.add(titleLabel);
        clickableViews.add(callToActionButton);
        _nativeAd.registerViewForInteraction(_nativeAdView, clickableViews);
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
