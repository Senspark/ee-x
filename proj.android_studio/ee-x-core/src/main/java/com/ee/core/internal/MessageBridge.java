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

    /**
     * Registered handlers.
     */
    private Map<String, MessageHandler> _handlers;

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
    public static MessageBridge getInstance() {
        return Holder.Instance;
    }

    private MessageBridge() {
        _handlers = new HashMap<>();
    }


    /**
     * Calls a handler from Java with a message.
     *
     * @param tag The unique tag of the handler.
     * @param msg The message.
     * @return Reply message from Java.
     */
    @NonNull
    private String call(@NonNull String tag, @NonNull String msg) {
        MessageHandler handler = _handlers.get(tag);
        if (handler == null) {
            _logger.error("call: " + tag + " doesn't exist!");
            return DictionaryUtils.emptyResult();
        }
        return handler.handle(msg);
    }

    /**
     * Statically calls a handler from Java with a message.
     *
     * @param tag The unique tag of the handler.
     * @param msg The message.
     * @return Reply message from Java.
     */
    @NonNull
    private static String staticCall(@NonNull String tag, @NonNull String msg) {
        return getInstance().call(tag, msg);
    }

    /**
     * Calls a handler from C++ without a message.
     *
     * @param tag The unique tag of the handler.
     * @return Reply message from c++.
     */
    @NonNull
    public String callCpp(@NonNull String tag) {
        return callCpp(tag, "");
    }

    /**
     * Calls a handler from C++ with a message.
     *
     * @param tag The unique tag of the handler.
     * @param msg The message.
     * @return Reply message from C++.
     */
    @NonNull
    public native String callCpp(@NonNull String tag, @NonNull String msg);

    /**
     * Registers a new handler to receive messages from C++.
     *
     * @param handler The handler.
     * @param tag     The unique tag of the handler.
     * @return Whether the registration was successful.
     */
    public boolean registerHandler(MessageHandler handler, @NonNull String tag) {
        if (_handlers.containsKey(tag)) {
            _logger.error("registerHandler: " + tag + " already exists!");
            return false;
        }
        _handlers.put(tag, handler);
        return true;
    }

    /**
     * Deregisters an existing handler not to receive messages from C++.
     *
     * @param tag The unique
     * @return Whether the deregistration was successful.
     */
    public boolean deregisterHandler(@NonNull String tag) {
        if (!_handlers.containsKey(tag)) {
            _logger.error("deregisterHandler: " + tag + " doesn't exist!");
            return false;
        }
        _handlers.remove(tag);
        return true;
    }
}
