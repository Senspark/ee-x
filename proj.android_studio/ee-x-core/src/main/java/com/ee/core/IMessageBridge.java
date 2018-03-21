package com.ee.core;

import android.support.annotation.NonNull;

/**
 * Created by eps on 3/20/18.
 */

public interface IMessageBridge {
    /**
     * Calls a handler from C++ without a message.
     *
     * @param tag The unique tag of the handler.
     * @return Reply message from c++.
     */
    @NonNull
    String callCpp(@NonNull String tag);

    /**
     * Calls a handler from C++ with a message.
     *
     * @param tag The unique tag of the handler.
     * @param msg The message.
     * @return Reply message from C++.
     */
    @NonNull
    String callCpp(@NonNull String tag, @NonNull String message);

    /**
     * Registers a new handler to receive messages from C++.
     *
     * @param handler The handler.
     * @param tag     The unique tag of the handler.
     * @return Whether the registration was successful.
     */
    boolean registerHandler(MessageHandler handler, @NonNull String tag);

    /**
     * Deregisters an existing handler not to receive messages from C++.
     *
     * @param tag The unique
     * @return Whether the deregistration was successful.
     */
    boolean deregisterHandler(@NonNull String tag);
}
