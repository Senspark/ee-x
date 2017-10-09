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

import com.ee.core.Logger;
import com.ee.core.internal.JsonUtils;
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
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Zinge on 10/9/17.
 */

public class FacebookNativeAd implements AdListener {
    private static final Logger _logger = new Logger(FacebookNativeAd.class.getName());

    private NativeAd                _nativeAd;
    private View                    _nativeAdView;
    private boolean                 _isAdLoaded;
    private Activity                _activity;
    private FacebookNativeAdBuilder _builder;

    public FacebookNativeAd(@NonNull Activity activity, @NonNull FacebookNativeAdBuilder builder) {
        _activity = activity;
        _builder = builder;

        _nativeAd = null;
        _nativeAdView = null;
        _isAdLoaded = false;
        NativeAd nativeAd = new NativeAd(activity, builder.adId);
        nativeAd.setAdListener(this);
        nativeAd.loadAd();
        _nativeAd = nativeAd;

        FrameLayout rootView = Utils.getRootView(activity);
        int layoutId = rootView
            .getResources()
            .getIdentifier(builder.layoutName, "layout", activity.getPackageName());
        View nativeAdView = LayoutInflater.from(activity).inflate(layoutId, null);
        nativeAdView.setVisibility(View.INVISIBLE);

        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        rootView.addView(nativeAdView);
        _nativeAdView = nativeAdView;

        registerHandlers();
    }

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        deregisterHandlers();
        _nativeAd.unregisterView();
        _nativeAd.destroy();
        _nativeAd = null;
        FrameLayout rootView = Utils.getRootView(_activity);
        rootView.removeView(_nativeAdView);
        _nativeAdView = null;
        _builder = null;
        _activity = null;
    }

    private String k__isLoaded() {
        return "FacebookNativeAd_isLoaded_" + _nativeAd.getPlacementId();
    }

    private String k__getPosition() {
        return "FacebookNativeAd_getPosition_" + _nativeAd.getPlacementId();
    }

    private String k__setPosition() {
        return "FacebookNativeAd_setPosition_" + _nativeAd.getPlacementId();
    }

    private String k__getSize() {
        return "FacebookNativeAd_getSize_" + _nativeAd.getPlacementId();
    }

    private String k__setSize() {
        return "FacebookNativeAd_setSize_" + _nativeAd.getPlacementId();
    }

    private String k__setVisible() {
        return "FacebookNativeAd_setVisible_" + _nativeAd.getPlacementId();
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return isLoaded() ? "true" : "false";
            }
        }, k__isLoaded());

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Point position = getPosition();
                Map<String, Object> dict = new HashMap<>();
                dict.put("x", position.x);
                dict.put("y", position.y);
                return JsonUtils.convertDictionaryToString(dict);
            }
        }, k__getPosition());

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                int x = (Integer) dict.get("x");
                int y = (Integer) dict.get("y");
                setPosition(new Point(x, y));
                return "";
            }
        }, k__setPosition());

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Point size = getSize();
                Map<String, Object> dict = new HashMap<>();
                dict.put("width", size.x);
                dict.put("height", size.y);
                return JsonUtils.convertDictionaryToString(dict);
            }
        }, k__getSize());

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                int x = (Integer) dict.get("width");
                int y = (Integer) dict.get("height");
                setSize(new Point(x, y));
                return "";
            }
        }, k__setSize());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setVisible(message.equals("true"));
                return "";
            }
        }, k__setVisible());
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__isLoaded());
        bridge.deregisterHandler(k__getPosition());
        bridge.deregisterHandler(k__setPosition());
        bridge.deregisterHandler(k__getSize());
        bridge.deregisterHandler(k__setSize());
        bridge.deregisterHandler(k__setVisible());
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isLoaded() {
        return _isAdLoaded;
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public Point getPosition() {
        int p[] = new int[2];
        _nativeAdView.getLocationInWindow(p);
        return new Point(p[0], p[1]);
    }

    @SuppressWarnings("WeakerAccess")
    public void setPosition(@NonNull Point position) {
        FrameLayout.LayoutParams params =
            (FrameLayout.LayoutParams) _nativeAdView.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        _nativeAdView.setLayoutParams(params);
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public Point getSize() {
        return new Point(_nativeAdView.getWidth(), _nativeAdView.getHeight());
    }

    @SuppressWarnings("WeakerAccess")
    public void setSize(@NonNull Point size) {
        FrameLayout.LayoutParams params =
            (FrameLayout.LayoutParams) _nativeAdView.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        _nativeAdView.setLayoutParams(params);
    }

    @SuppressWarnings("WeakerAccess")
    public void setVisible(boolean visible) {
        if (visible) {
            _nativeAdView.setVisibility(View.VISIBLE);
        } else {
            _nativeAdView.setVisibility(View.INVISIBLE);
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
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
    }
}
