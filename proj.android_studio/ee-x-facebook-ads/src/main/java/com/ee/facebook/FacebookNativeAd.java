package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
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
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdChoicesView;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.MediaView;
import com.facebook.ads.NativeAd;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookNativeAd implements AdListener, AdViewInterface {
    private static final String k__ad_choices     = "ad_choices";
    private static final String k__body           = "body";
    private static final String k__call_to_action = "call_to_action";
    private static final String k__icon           = "icon";
    private static final String k__media          = "media";
    private static final String k__social_context = "social_context";
    private static final String k__title          = "title";

    private static final Logger _logger = new Logger(FacebookNativeAd.class.getName());

    private NativeAd            _nativeAd;
    private View                _nativeAdView;
    private boolean             _isAdLoaded;
    private Context             _context;
    private Activity            _activity;
    private String              _adId;
    private String              _layoutName;
    private Map<String, String> _identifiers;
    private AdViewHelper        _helper;

    public FacebookNativeAd(@NonNull Context context, @Nullable Activity activity,
                            @NonNull String adId, @NonNull String layoutName,
                            @NonNull Map<String, String> identifiers) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _adId = adId;
        _layoutName = layoutName;
        _identifiers = identifiers;
        _nativeAd = null;
        _nativeAdView = null;
        _isAdLoaded = false;
        _helper = new AdViewHelper("FacebookNativeAd", adId);

        createInternalAd();
        createView();
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

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        destroyView();
        _adId = null;
        _layoutName = null;
        _identifiers = null;
        _helper = null;
        _context = null;
    }

    @NonNull
    private String k__createInternalAd() {
        return "FacebookNativeAd_createInternalAd_" + _adId;
    }

    @NonNull
    private String k__destroyInternalAd() {
        return "FacebookNativeAd_destroyInternalAd_" + _adId;
    }

    @NonNull
    private String k__onLoaded() {
        return "FacebookNativeAd_onLoaded_" + _adId;
    }

    @NonNull
    private String k__onFailedToLoad() {
        return "FacebookNativeAd_onFailedToLoad_" + _adId;
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
        if (_nativeAd != null) {
            return false;
        }
        _isAdLoaded = false;
        NativeAd nativeAd = new NativeAd(_context, _adId);
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
        int layoutId =
            _context.getResources().getIdentifier(_layoutName, "layout", _context.getPackageName());
        View nativeAdView = LayoutInflater.from(_context).inflate(layoutId, null);
        nativeAdView.setVisibility(View.INVISIBLE);

        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        nativeAdView.setLayoutParams(params);

        _nativeAdView = nativeAdView;
        if (_activity != null) {
            addToActivity(_activity);
        }
    }

    private void destroyView() {
        Utils.checkMainThread();
        removeFromActivity(_activity);
        _nativeAdView = null;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_nativeAdView);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_nativeAdView);
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
        _logger.info("load");
        _nativeAd.loadAd();
    }

    @NonNull
    @Override
    public Point getPosition() {
        return AdViewHelper.getPosition(_nativeAdView);
    }

    @Override
    public void setPosition(@NonNull Point position) {
        AdViewHelper.setPosition(position, _nativeAdView);
    }

    @NonNull
    @Override
    public Point getSize() {
        return AdViewHelper.getSize(_nativeAdView);
    }

    @Override
    public void setSize(@NonNull Point size) {
        AdViewHelper.setSize(size, _nativeAdView);
    }

    @Override
    public void setVisible(boolean visible) {
        AdViewHelper.setVisible(visible, _nativeAdView);
    }

    private int getIdentifier(@NonNull String identifier) {
        if (!_identifiers.containsKey(identifier)) {
            return 0;
        }
        Resources resources = _context.getResources();
        return resources.getIdentifier(_identifiers.get(identifier), "id",
            _context.getPackageName());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onAdLoaded: " + adError.getErrorMessage());
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();

        _nativeAd.unregisterView();

        TextView bodyLabel = _nativeAdView.findViewById(getIdentifier(k__body));
        Button callToActionButton = _nativeAdView.findViewById(getIdentifier(k__call_to_action));
        ImageView iconImage = _nativeAdView.findViewById(getIdentifier(k__icon));
        MediaView mediaView = _nativeAdView.findViewById(getIdentifier(k__media));
        TextView socialContextLabel = _nativeAdView.findViewById(getIdentifier(k__social_context));
        TextView titleLabel = _nativeAdView.findViewById(getIdentifier(k__title));

        NativeAd.downloadAndDisplayImage(_nativeAd.getAdIcon(), iconImage);
        bodyLabel.setText(_nativeAd.getAdBody());
        callToActionButton.setText(_nativeAd.getAdCallToAction());
        socialContextLabel.setText(_nativeAd.getAdSocialContext());
        titleLabel.setText(_nativeAd.getAdTitle());

        if (mediaView != null) {
            mediaView.setNativeAd(_nativeAd);
        }

        LinearLayout adChoicesViews = _nativeAdView.findViewById(getIdentifier(k__ad_choices));
        adChoicesViews.addView(new AdChoicesView(_context, _nativeAd, true));

        List<View> clickableViews = new ArrayList<>();
        clickableViews.add(titleLabel);
        clickableViews.add(callToActionButton);
        _nativeAd.registerViewForInteraction(_nativeAdView, clickableViews);

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
