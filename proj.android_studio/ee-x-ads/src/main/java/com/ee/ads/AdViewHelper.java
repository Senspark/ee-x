package com.ee.ads;

import androidx.annotation.NonNull;

import android.graphics.Point;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.JsonUtils;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 10/12/17.
 */

public class AdViewHelper {
    private IMessageBridge _bridge;
    private IAdView _view;
    private String _prefix;
    private String _adId;

    public AdViewHelper(@NonNull IMessageBridge bridge,
                        @NonNull IAdView view,
                        @NonNull String prefix,
                        @NonNull String adId) {
        _bridge = bridge;
        _view = view;
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
    private String k__isVisible() {
        return _prefix + "_isVisible_" + _adId;
    }

    @NonNull
    private String k__setVisible() {
        return _prefix + "_setVisible_" + _adId;
    }

    public void registerHandlers() {
        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(_view.isLoaded());
            }
        }, k__isLoaded());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                _view.load();
                return "";
            }
        }, k__load());

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Point position = _view.getPosition();
                Map<String, Object> dict = new HashMap<>();
                dict.put("x", position.x);
                dict.put("y", position.y);
                return JsonUtils.convertDictionaryToString(dict);
            }
        }, k__getPosition());

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                int x = (Integer) dict.get("x");
                int y = (Integer) dict.get("y");
                _view.setPosition(new Point(x, y));
                return "";
            }
        }, k__setPosition());

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Point size = _view.getSize();
                Map<String, Object> dict = new HashMap<>();
                dict.put("width", size.x);
                dict.put("height", size.y);
                return JsonUtils.convertDictionaryToString(dict);
            }
        }, k__getSize());

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("ConstantConditions")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                int x = (Integer) dict.get("width");
                int y = (Integer) dict.get("height");
                _view.setSize(new Point(x, y));
                return "";
            }
        }, k__setSize());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(_view.isVisible());
            }
        }, k__isVisible());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(_view.isVisible());
            }
        }, k__isVisible());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                _view.setVisible(Utils.toBoolean(message));
                return "";
            }
        }, k__setVisible());
    }

    public void deregisterHandlers() {
        _bridge.deregisterHandler(k__isLoaded());
        _bridge.deregisterHandler(k__load());
        _bridge.deregisterHandler(k__getPosition());
        _bridge.deregisterHandler(k__setPosition());
        _bridge.deregisterHandler(k__getSize());
        _bridge.deregisterHandler(k__setSize());
        _bridge.deregisterHandler(k__isVisible());
        _bridge.deregisterHandler(k__setVisible());
    }
}
