package com.ee.internal

import com.ee.AsyncMessageHandler
import com.ee.AsyncMessageResolver
import com.ee.IMessageBridge
import com.ee.Logger
import com.ee.MessageHandler
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 3/29/17.
 */
class MessageBridge private constructor() : IMessageBridge {
    companion object {
        private val _logger = Logger(MessageBridge::class.java.name)

        /**
         * Thread-safe singleton pattern.
         * https://stackoverflow.com/questions/51834996/singleton-class-in-kotlin
         */
        private val _sharedInstance = MessageBridge()

        /**
         * Gets a message bridge instance.
         *
         * @return A message bridge instance.
         */
        @JvmStatic
        fun getInstance(): IMessageBridge {
            return _sharedInstance;
        }

        fun staticCall(tag: String, message: String): String {
            return _sharedInstance.call(tag, message)
        }
    }

    /**
     * Registered handlers.
     */
    private val _handlers: MutableMap<String, MessageHandler>
    private val _handlerLock: Any

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
                handler.handle(request.message, object : AsyncMessageResolver {
                    override fun resolve(message: String) {
                        callCpp(request.callback_tag, message)
                    }
                })
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

    override fun callCpp(tag: String): String {
        return callCpp(tag, "")
    }


    override fun callCpp(tag: String, message: String): String {
        return ee_callCppInternal(tag, message)
    }

    /**
     * Calls a handler from Java with a message.
     *
     * @param tag     The unique tag of the handler.
     * @param message The message.
     * @return Reply message from Java.
     */
    private fun call(tag: String, message: String): String {
        val handler = findHandler(tag)
        if (handler == null) {
            _logger.error("call: $tag doesn't exist!")
            assertThat(false).isTrue()
            return ""
        }
        return handler.handle(message)
    }
}

private external fun ee_callCppInternal(tag: String, message: String): String

@NativeThread
@Suppress("unused")
private fun ee_staticCall(tag: String, message: String): String {
    return MessageBridge.staticCall(tag, message)
}