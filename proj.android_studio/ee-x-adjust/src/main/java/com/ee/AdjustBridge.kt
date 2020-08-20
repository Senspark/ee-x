package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.adjust.sdk.Adjust
import com.adjust.sdk.AdjustConfig
import com.adjust.sdk.AdjustEvent
import com.adjust.sdk.LogLevel
import com.ee.internal.deserialize
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

@ImplicitReflectionSerializer
@UnstableDefault
class AdjustBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(AdjustBridge::class.java.name)

        private const val kPrefix = "AdjustBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetEnabled = "${kPrefix}SetEnabled"
        private const val kGetAdvertisingIdentifier = "${kPrefix}GetAdvertisingIdentifier";
        private const val kGetDeviceIdentifier = "${kPrefix}GetDeviceIdentifier";
        private const val kTrackEvent = "${kPrefix}TrackEvent"
    }

    init {
        _logger.debug("constructor begin: context = $_context")
        registerHandlers()
        _logger.debug("constructor end.")
    }


    override fun onCreate(activity: Activity) {}

    override fun onStart() {}
    override fun onStop() {}

    override fun onResume() {
        Adjust.onResume()
    }

    override fun onPause() {
        Adjust.onPause()
    }

    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            @Serializable
            class Request(
                val token: String,
                val environment: Int,
                val logLevel: Int,
                val eventBufferingEnabled: Boolean,
                val useAppSecret: Boolean,
                val secretId: Long,
                val info1: Long,
                val info2: Long,
                val info3: Long,
                val info4: Long
            )

            val request = deserialize<Request>(message)
            initialize(request.token, parseEnvironment(request.environment),
                parseLogLevel(request.logLevel), request.eventBufferingEnabled,
                request.useAppSecret, request.secretId, request.info1, request.info2, request.info3, request.info4)
            ""
        }
        _bridge.registerHandler(kSetEnabled) { message ->
            setEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerAsyncHandler(kGetAdvertisingIdentifier) {
            suspendCoroutine { cont ->
                Adjust.getGoogleAdId(_context) { googleAdId ->
                    cont.resume(googleAdId ?: "")
                }
            }
        }
        _bridge.registerHandler(kGetDeviceIdentifier) {
            Adjust.getAdid()
        }
        _bridge.registerHandler(kTrackEvent) { message ->
            trackEvent(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetEnabled)
        _bridge.deregisterHandler(kGetAdvertisingIdentifier)
        _bridge.deregisterHandler(kGetDeviceIdentifier)
        _bridge.deregisterHandler(kTrackEvent)
    }

    @AnyThread
    fun parseEnvironment(environment: Int): String {
        if (environment == 0) {
            return AdjustConfig.ENVIRONMENT_SANDBOX
        }
        if (environment == 1) {
            return AdjustConfig.ENVIRONMENT_PRODUCTION
        }
        return ""
    }

    @AnyThread
    fun parseLogLevel(logLevel: Int): LogLevel {
        val dict = mapOf(
            0 to LogLevel.VERBOSE,
            1 to LogLevel.DEBUG,
            2 to LogLevel.INFO,
            3 to LogLevel.WARN,
            4 to LogLevel.ERROR,
            5 to LogLevel.ASSERT,
            6 to LogLevel.SUPRESS
        )
        return dict[logLevel] ?: LogLevel.VERBOSE
    }

    @AnyThread
    fun initialize(token: String, environment: String,
                   logLevel: LogLevel, eventBufferingEnabled: Boolean,
                   useAppSecret: Boolean, secretId: Long, info1: Long, info2: Long, info3: Long, info4: Long) {
        val config = AdjustConfig(_context, token, environment)
        config.setLogLevel(logLevel)
        config.setEventBufferingEnabled(eventBufferingEnabled)
        if (useAppSecret) {
            config.setAppSecret(secretId, info1, info2, info3, info4)
        }
        Adjust.onCreate(config)
    }

    @AnyThread
    fun setEnabled(enabled: Boolean) {
        Adjust.setEnabled(enabled)
    }

    @AnyThread
    fun trackEvent(token: String) {
        val event = AdjustEvent(token)
        Adjust.trackEvent(event)
    }
}