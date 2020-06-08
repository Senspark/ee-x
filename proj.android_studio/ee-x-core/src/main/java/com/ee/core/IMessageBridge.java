package com.ee.core;

import androidx.annotation.NonNull;

/**
 * Created by eps on 3/20/18.
 */

public interface IMessageBridge {
    /**
     * Registers a new handler to receive messages from C++.
     *
     * @param tag     The unique tag of the handler.
     * @param handler The handler.
     * @return Whether the registration was successful.
     */
    boolean registerHandler(@NonNull String tag, @NonNull MessageHandler handler);

    /**
     * Registers a new async handler to receive messages from C++.
     *
     * @param tag     The unique tag of the handler.
     * @param handler The handler.
     * @return Whether the registration was successful.
     */
    boolean registerAsyncHandler(@NonNull String tag, @NonNull AsyncMessageHandler handler);

    /**
     * Deregisters an existing handler not to receive messages from C++.
     *
     * @param tag The unique
     * @return Whether the deregistration was successful.
     */
    boolean deregisterHandler(@NonNull String tag);

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
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from C++.
     */
    @NonNull
    String callCpp(@NonNull String tag, @NonNull String message);
}
