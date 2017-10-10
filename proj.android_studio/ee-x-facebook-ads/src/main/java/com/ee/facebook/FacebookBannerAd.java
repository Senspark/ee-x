package com.ee.facebook;

import android.app.Activity;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;

import com.ee.core.Logger;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdListener;
import com.facebook.ads.AdSize;
import com.facebook.ads.AdView;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 10/9/17.
 */

public class FacebookBannerAd implements AdListener {
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

    private AdView  _adView;
    private boolean _isAdLoaded;

    public FacebookBannerAd(@NonNull Activity activity, @NonNull String adId,
                            @NonNull AdSize adSize) {
        _isAdLoaded = false;

        _adView = new AdView(activity, adId, adSize);
        _adView.setAdListener(this);
        _adView.loadAd();

        FrameLayout rootView = Utils.getRootView(activity);
        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        rootView.addView(_adView, params);

        registerHandlers();
    }

    public void destroy() {
        deregisterHandlers();
        _adView.destroy();
        _adView = null;
    }

    private String k__load() {
        return "FacebookBannerAd_load_" + _adView.getPlacementId();
    }

    private String k__isLoaded() {
        return "FacebookBannerAd_isLoaded_" + _adView.getPlacementId();
    }

    private String k__getPosition() {
        return "FacebookBannerAd_getPosition_" + _adView.getPlacementId();
    }

    private String k__setPosition() {
        return "FacebookBannerAd_setPosition_" + _adView.getPlacementId();
    }

    private String k__getSize() {
        return "FacebookBannerAd_getSize_" + _adView.getPlacementId();
    }

    private String k__setSize() {
        return "FacebookBannerAd_setSize_" + _adView.getPlacementId();
    }

    private String k__setVisible() {
        return "FacebookBannerAd_setVisible_" + _adView.getPlacementId();
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                load();
                return "";
            }
        }, k__load());

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
    public void load() {
        _adView.loadAd();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isLoaded() {
        return _isAdLoaded;
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public Point getPosition() {
        int p[] = new int[2];
        _adView.getLocationInWindow(p);
        return new Point(p[0], p[1]);
    }

    @SuppressWarnings("WeakerAccess")
    public void setPosition(@NonNull Point position) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _adView.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        _adView.setLayoutParams(params);
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public Point getSize() {
        return new Point(_adView.getWidth(), _adView.getHeight());
    }

    @SuppressWarnings("WeakerAccess")
    public void setSize(@NonNull Point size) {
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _adView.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        _adView.setLayoutParams(params);
    }

    @SuppressWarnings("WeakerAccess")
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
