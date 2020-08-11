package com.ee.internal

import com.ee.AsyncMessageHandler
import com.ee.IMessageBridge
import com.ee.Logger
import com.ee.MessageHandler
import com.ee.PluginManager
import com.google.common.truth.Truth.assertThat
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.launch
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

interface MessageBridgeHandler {
    fun callCpp(tag: String, message: String): String
}

/**
 * Created by Zinge on 3/29/17.
 */
class MessageBridge constructor(private val _handler: MessageBridgeHandler) : IMessageBridge {
    companion object {
        private val _logger = Logger(MessageBridge::class.java.name)
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
                _logger.error("registerHandler: $tag already exists!")
                assertThat(false).isTrue()
                return false
            }
            _handlers[tag] = handler
            return true
        }
    }

    @UnstableDefault
    @ImplicitReflectionSerializer
    override fun registerAsyncHandler(tag: String, handler: AsyncMessageHandler): Boolean {
        return registerHandler(tag, object : MessageHandler {
            override fun handle(message: String): String {
                @Serializable
                class Request(
                    val callback_tag: String,
                    val message: String
                )

                val request = deserialize<Request>(message)
                _scope.launch {
                    val response = handler.handle(request.message)
                    callCpp(request.callback_tag, response)
                }
                return ""
            }
        })
    }

    override fun deregisterHandler(tag: String): Boolean {
        synchronized(_handlerLock) {
            if (!_handlers.containsKey(tag)) {
                _logger.error("deregisterHandler: $tag doesn't exist!")
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
            _logger.error("call: $tag doesn't exist!")
            assertThat(false).isTrue()
            return ""
        }
        return handler.handle(message)
    }

    override fun callCpp(tag: String): String {
        return callCpp(tag, "")
    }

    override fun callCpp(tag: String, message: String): String {
        return _handler.callCpp(tag, message)
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