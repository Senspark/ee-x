package com.ee.core.internal;

import android.support.annotation.NonNull;

import com.ee.core.Logger;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 3/29/17.
 */

public class MessageBridge {
    private static final Logger _logger = new Logger(MessageBridge.class.getName());

    private Map<String, MessageHandler> _handlers;

    private static class Holder {
        private static final MessageBridge Instance = new MessageBridge();
    }

    public static MessageBridge getInstance() {
        return Holder.Instance;
    }

    private MessageBridge() {
        _handlers = new HashMap<>();
    }

    @NonNull
    private String call(@NonNull String tag, @NonNull String msg) {
        MessageHandler handler = _handlers.get(tag);
        if (handler == null) {
            _logger.error("call: " + tag + " doesn't exist!");
            return DictionaryUtils.emptyResult();
        }
        return handler.handle(msg);
    }

    @NonNull
    private static String staticCall(@NonNull String tag, @NonNull String msg) {
        return getInstance().call(tag, msg);
    }

    @NonNull
    public native String callCpp(@NonNull String tag, @NonNull String msg);

    public void registerHandler(MessageHandler handler, @NonNull String tag) {
        if (_handlers.containsKey(tag)) {
            _logger.error("registerHandler: " + tag + " already exists!");
            return;
        }
        _handlers.put(tag, handler);
    }

    public void deregisterHandler(@NonNull String tag) {
        if (!_handlers.containsKey(tag)) {
            _logger.error("deregisterHandler: " + tag + " doesn't exist!");
            return;
        }
        _handlers.remove(tag);
    }
}
