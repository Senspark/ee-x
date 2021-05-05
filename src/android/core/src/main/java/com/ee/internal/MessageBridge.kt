package com.ee.internal

import com.ee.AsyncMessageHandler
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.MessageHandler
import com.ee.PluginManager
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import kotlinx.serialization.Serializable

typealias MessageBridgeHandler = (tag: String, message: String) -> Unit

/**
 * Created by Zinge on 3/29/17.
 */
class MessageBridge(
    private val _logger: ILogger,
    private val _handler: MessageBridgeHandler)
    : IMessageBridge {
    companion object {
        private val kTag = MessageBridge::class.java.name
    }

    /**
     * Registered handlers.
     */
    private val _handlers: MutableMap<String, MessageHandler>
    private val _handlerLock: Any
    private val _scope = MainScope()

    init {
        _handlers = HashMap()
        _handlerLock = Any()
    }

    override fun registerHandler(tag: String, handler: MessageHandler) {
        synchronized(_handlerLock) {
            if (_handlers.containsKey(tag)) {
                throw IllegalArgumentException("$kTag: ${this::registerHandler.name}: $tag already exists!")
            }
            _handlers[tag] = handler
        }
    }

    @Serializable
    private class Request(
        val callback_tag: String,
        val message: String
    )

    override fun registerAsyncHandler(tag: String, handler: AsyncMessageHandler) {
        return registerHandler(tag) { message ->
            val request = deserialize<Request>(message)
            _scope.launch {
                val response = handler(request.message)
                callCpp(request.callback_tag, response)
            }
            ""
        }
    }

    override fun deregisterHandler(tag: String) {
        synchronized(_handlerLock) {
            if (!_handlers.containsKey(tag)) {
                throw IllegalArgumentException("$kTag: ${this::deregisterHandler.name}: $tag doesn't exist!")
            }
            _handlers.remove(tag)
        }
    }

    private fun findHandler(tag: String): MessageHandler? {
        synchronized(_handlerLock) {
            return _handlers[tag]
        }
    }

    override fun call(tag: String, message: String): String {
        val handler = findHandler(tag)
            ?: throw IllegalArgumentException("$kTag: ${this::call.name}: $tag doesn't exist!")
        return handler(message)
    }

    override fun callCpp(tag: String) {
        callCpp(tag, "")
    }

    override fun callCpp(tag: String, message: String) {
        _handler(tag, message)
    }
}

@NativeThread
@Suppress("FunctionName", "unused")
private fun ee_staticCall(tag: String, message: String): String {
    val bridge = PluginManager.instance.bridge ?: throw IllegalStateException("Bridge is null")
    return bridge.call(tag, message)
}