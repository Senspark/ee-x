package com.ee.ads;

import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.View;
import android.widget.FrameLayout;

import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 10/12/17.
 */

public class AdViewHelper {
    private String _prefix;
    private String _adId;

    public AdViewHelper(String prefix, String adId) {
        _prefix = prefix;
        _adId = adId;
    }

    @NonNull
    private String k__isLoaded() {
        return _prefix + "_isLoaded_" + _adId;
    }

    @NonNull
    private String k__load() {
        return _prefix + "_load_" + _adId;
    }

    @NonNull
    private String k__getPosition() {
        return _prefix + "_getPosition_" + _adId;
    }

    @NonNull
    private String k__setPosition() {
        return _prefix + "_setPosition_" + _adId;
    }

    @NonNull
    private String k__getSize() {
        return _prefix + "_getSize_" + _adId;
    }

    @NonNull
    private String k__setSize() {
        return _prefix + "_setSize_" + _adId;
    }

    @NonNull
    private String k__setVisible() {
        return _prefix + "_setVisible_" + _adId;
    }

    public void registerHandlers(final AdViewInterface adView) {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return adView.isLoaded() ? "true" : "false";
            }
        }, k__isLoaded());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                adView.load();
                return "";
            }
        }, k__load());

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Point position = adView.getPosition();
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
                adView.setPosition(new Point(x, y));
                return "";
            }
        }, k__setPosition());

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Point size = adView.getSize();
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
                adView.setSize(new Point(x, y));
                return "";
            }
        }, k__setSize());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                adView.setVisible(message.equals("true"));
                return "";
            }
        }, k__setVisible());
    }

    public void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__isLoaded());
        bridge.deregisterHandler(k__load());
        bridge.deregisterHandler(k__getPosition());
        bridge.deregisterHandler(k__setPosition());
        bridge.deregisterHandler(k__getSize());
        bridge.deregisterHandler(k__setSize());
        bridge.deregisterHandler(k__setVisible());
    }

    @NonNull
    public static Point getPosition(@NonNull View view) {
        Utils.checkMainThread();
        /*
        int p[] = new int[2];
        view.getLocationInWindow(p);
        return new Point(p[0], p[1]);
        */
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) view.getLayoutParams();
        return new Point(params.leftMargin, params.topMargin);
    }

    public static void setPosition(@NonNull Point position, @NonNull View view) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) view.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        view.setLayoutParams(params);
    }

    @NonNull
    public static Point getSize(@NonNull View view) {
        Utils.checkMainThread();
        // return new Point(view.getWidth(), view.getHeight());
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) view.getLayoutParams();
        return new Point(params.width, params.height);
    }

    public static void setSize(@NonNull Point size, @NonNull View view) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) view.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        view.setLayoutParams(params);
    }

    public static void setVisible(boolean visible, @NonNull View view) {
        Utils.checkMainThread();
        if (visible) {
            view.setVisibility(View.VISIBLE);
        } else {
            view.setVisibility(View.INVISIBLE);
        }
    }
}
