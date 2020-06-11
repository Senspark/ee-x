package com.ee.core

import kotlinx.serialization.json.Json

interface MessageHandler {
    fun handle(message: String): String
}

interface AsyncMessageResolver {
    fun resolve(message: String)
}

interface AsyncMessageHandler {
    fun handle(message: String, resolver: AsyncMessageResolver)
}

/**
 * Created by eps on 3/20/18.
 */
interface IMessageBridge {
    /**
     * Registers a new handler to receive messages from C++.
     *
     * @param tag     The unique tag of the handler.
     * @param handler The handler.
     * @return Whether the registration was successful.
     */
    fun registerHandler(tag: String, handler: MessageHandler): Boolean

    /**
     * Registers a new async handler to receive messages from C++.
     *
     * @param tag     The unique tag of the handler.
     * @param handler The handler.
     * @return Whether the registration was successful.
     */
    fun registerAsyncHandler(tag: String, handler: AsyncMessageHandler): Boolean

    /**
     * Deregisters an existing handler not to receive messages from C++.
     *
     * @param tag The unique
     * @return Whether the deregistration was successful.
     */
    fun deregisterHandler(tag: String): Boolean

    /**
     * Calls a handler from C++ without a message.
     *
     * @param tag The unique tag of the handler.
     * @return Reply message from c++.
     */
    fun callCpp(tag: String): String

    /**
     * Calls a handler from C++ with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from C++.
     */
    fun callCpp(tag: String, message: String): String
}

// https://stackoverflow.com/questions/33590646/kotlin-use-a-lambda-in-place-of-a-functional-interface
inline fun IMessageBridge.registerHandler(
    tag: String, crossinline handler: (string: String) -> String): Boolean {
    return registerHandler(tag, object : MessageHandler {
        override fun handle(message: String): String {
            return handler(message)
        }
    })
}

inline fun IMessageBridge.registerAsyncHandler(
    tag: String, crossinline handler: (message: String, resolver: AsyncMessageResolver) -> Unit): Boolean {
    return registerAsyncHandler(tag, object : AsyncMessageHandler {
        override fun handle(message: String, resolver: AsyncMessageResolver) {
            handler(message, resolver)
        }
    })
}