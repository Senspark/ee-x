package com.ee

import androidx.annotation.AnyThread

typealias MessageHandler = (message: String) -> String
typealias AsyncMessageHandler = suspend (message: String) -> String

/**
 * Created by eps on 3/20/18.
 */
interface IMessageBridge {
    /**
     * Registers a new handler to receive messages from C++.
     *
     * @param tag     The unique tag of the handler.
     * @param handler The handler.
     */
    @AnyThread
    fun registerHandler(tag: String, handler: MessageHandler)

    /**
     * Registers a new async handler to receive messages from C++.
     *
     * @param tag     The unique tag of the handler.
     * @param handler The handler.
     */
    @AnyThread
    fun registerAsyncHandler(tag: String, handler: AsyncMessageHandler)

    /**
     * Deregisters an existing handler not to receive messages from C++.
     *
     * @param tag The unique tag of the handler.
     */
    @AnyThread
    fun deregisterHandler(tag: String)

    /**
     * Calls a handler from Java with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from Java.
     */
    @AnyThread
    fun call(tag: String, message: String): String

    /**
     * Calls a handler from C++ without a message.
     *
     * @param tag The unique tag of the handler.
     */
    @AnyThread
    fun callCpp(tag: String)

    /**
     * Calls a handler from C++ with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     */
    @AnyThread
    fun callCpp(tag: String, message: String)
}