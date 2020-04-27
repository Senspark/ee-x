package com.ee.ads;

import android.graphics.Point;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/12/17.
 */

public class AdViewHelper {
    private IMessageBridge _bridge;
    private IAdView _view;
    private MessageHelper _helper;

    public AdViewHelper(@NonNull IMessageBridge bridge,
                        @NonNull IAdView view,
                        @NonNull MessageHelper helper) {
        _bridge = bridge;
        _view = view;
        _helper = helper;
    }

    public void registerHandlers() {
        _bridge.registerHandler(message ->
            Utils.toString(_view.isLoaded()), _helper.isLoaded());

        _bridge.registerHandler(message -> {
            _view.load();
            return "";
        }, _helper.load());

        _bridge.registerHandler(message -> {
            Point position = _view.getPosition();
            Map<String, Object> dict = new HashMap<>();
            dict.put("x", position.x);
            dict.put("y", position.y);
            return Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict));
        }, _helper.getPosition());

        _bridge.registerHandler(message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            Integer x = (Integer) dict.get("x");
            Integer y = (Integer) dict.get("y");
            assertThat(x).isNotNull();
            assertThat(y).isNotNull();

            _view.setPosition(new Point(x, y));
            return "";
        }, _helper.setPosition());

        _bridge.registerHandler(message -> {
            Point size = _view.getSize();
            Map<String, Object> dict = new HashMap<>();
            dict.put("width", size.x);
            dict.put("height", size.y);
            return Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict));
        }, _helper.getSize());

        _bridge.registerHandler(message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            Integer x = (Integer) dict.get("width");
            Integer y = (Integer) dict.get("height");
            assertThat(x).isNotNull();
            assertThat(y).isNotNull();

            _view.setSize(new Point(x, y));
            return "";
        }, _helper.setSize());

        _bridge.registerHandler(message ->
            Utils.toString(_view.isVisible()), _helper.isVisible());

        _bridge.registerHandler(message -> {
            _view.setVisible(Utils.toBoolean(message));
            return "";
        }, _helper.setVisible());
    }

    public void deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded());
        _bridge.deregisterHandler(_helper.load());
        _bridge.deregisterHandler(_helper.getPosition());
        _bridge.deregisterHandler(_helper.setPosition());
        _bridge.deregisterHandler(_helper.getSize());
        _bridge.deregisterHandler(_helper.setSize());
        _bridge.deregisterHandler(_helper.isVisible());
        _bridge.deregisterHandler(_helper.setVisible());
    }
}
