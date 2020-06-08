package com.ee.core;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.core.internal.JsonUtils;

import java.util.HashMap;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 3/29/17.
 */

public class MessageBridge implements IMessageBridge {
    private static final Logger _logger = new Logger(MessageBridge.class.getName());

    /**
     * Registered handlers.
     */
    private final Map<String, MessageHandler> _handlers;

    private final Object _handlerLock;

    /**
     * Thread-safe singleton pattern.
     */
    private static class Holder {
        private static final MessageBridge Instance = new MessageBridge();
    }

    /**
     * Gets a message bridge instance.
     *
     * @return A message bridge instance.
     */
    public static IMessageBridge getInstance() {
        return Holder.Instance;
    }

    private MessageBridge() {
        _handlers = new HashMap<>();
        _handlerLock = new Object();
    }

    public boolean registerHandler(@NonNull String tag, @NonNull MessageHandler handler) {
        synchronized (_handlerLock) {
            if (_handlers.containsKey(tag)) {
                _logger.error("registerHandler: " + tag + " already exists!");
                return false;
            }
            _handlers.put(tag, handler);
            return true;
        }
    }

    public boolean registerAsyncHandler(@NonNull String tag, @NonNull AsyncMessageHandler handler) {
        return registerHandler(tag, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String callbackTag = (String) dict.get("callback_tag");
            String originalMessage = (String) dict.get("message");
            assertThat(callbackTag).isNotNull();
            assertThat(originalMessage).isNotNull();

            handler.handle(originalMessage, callbackMessage ->
                callCpp(callbackTag, callbackMessage));
            return "";
        });
    }

    public boolean deregisterHandler(@NonNull String tag) {
        synchronized (_handlerLock) {
            if (!_handlers.containsKey(tag)) {
                _logger.error("deregisterHandler: " + tag + " doesn't exist!");
                return false;
            }
            _handlers.remove(tag);
            return true;
        }
    }

    @Nullable
    private MessageHandler findHandler(@NonNull String tag) {
        synchronized (_handlerLock) {
            if (!_handlers.containsKey(tag)) {
                return null;
            }
            return _handlers.get(tag);
        }
    }

    @NonNull
    public String callCpp(@NonNull String tag) {
        return callCpp(tag, "");
    }

    @NonNull
    public String callCpp(@NonNull String tag, @NonNull String message) {
        return callCppInternal(tag, message);
    }

    /**
     * Calls a handler from C++ with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from C++.
     */
    @NonNull
    private native String callCppInternal(@NonNull String tag, @NonNull String message);

    /**
     * Calls a handler from Java with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from Java.
     */
    @NonNull
    private String call(@NonNull String tag, @NonNull String message) {
        MessageHandler handler = findHandler(tag);
        if (handler == null) {
            _logger.error("call: " + tag + " doesn't exist!");
            return "";
        }
        return handler.handle(message);
    }

    /**
     * Statically calls a handler from Java with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from Java.
     */
    @NonNull
    private static String staticCall(@NonNull String tag, @NonNull String message) {
        return Holder.Instance.call(tag, message);
    }
}
