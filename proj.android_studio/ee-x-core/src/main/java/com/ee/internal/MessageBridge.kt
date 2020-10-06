package com.ee.internal

import com.ee.AsyncMessageHandler
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.MessageHandler
import com.ee.PluginManager
import com.google.common.truth.Truth.assertThat
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable

typealias MessageBridgeHandler = (tag: String, message: String) -> String

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

    override fun registerHandler(tag: String, handler: MessageHandler): Boolean {
        synchronized(_handlerLock) {
            if (_handlers.containsKey(tag)) {
                _logger.error("$kTag: registerHandler: $tag already exists!")
                assertThat(false).isTrue()
                return false
            }
            _handlers[tag] = handler
            return true
        }
    }

    @InternalSerializationApi
    override fun registerAsyncHandler(tag: String, handler: AsyncMessageHandler): Boolean {
        return registerHandler(tag) { message ->
            @Serializable
            class Request(
                val callback_tag: String,
                val message: String
            )

            val request = deserialize<Request>(message)
            _scope.launch {
                val response = handler(request.message)
                callCpp(request.callback_tag, response)
            }
            ""
        }
    }

    override fun deregisterHandler(tag: String): Boolean {
        synchronized(_handlerLock) {
            if (!_handlers.containsKey(tag)) {
                _logger.error("$kTag: deregisterHandler: $tag doesn't exist!")
                assertThat(false).isTrue()
                return false
            }
            _handlers.remove(tag)
            return true
        }
    }

    private fun findHandler(tag: String): MessageHandler? {
        synchronized(_handlerLock) {
            return _handlers[tag]
        }
    }

    override fun call(tag: String, message: String): String {
        val handler = findHandler(tag)
        if (handler == null) {
            _logger.error("$kTag: call: $tag doesn't exist!")
            assertThat(false).isTrue()
            return ""
        }
        return handler(message)
    }

    override fun callCpp(tag: String): String {
        return callCpp(tag, "")
    }

    override fun callCpp(tag: String, message: String): String {
        return _handler(tag, message)
    }
}

internal external fun ee_callCppInternal(tag: String, message: String): String

@NativeThread
@Suppress("unused")
private fun ee_staticCall(tag: String, message: String): String {
    val bridge = PluginManager.getInstance().getBridge()
        ?: throw IllegalStateException("Bridge is null")
    return bridge.call(tag, message)
}