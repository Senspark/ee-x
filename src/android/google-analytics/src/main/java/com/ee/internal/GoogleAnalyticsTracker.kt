package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.google.android.gms.analytics.GoogleAnalytics
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable

/**
 * Created by Zinge on 10/25/17.
 */
@InternalSerializationApi
class GoogleAnalyticsTracker(
    private val _bridge: IMessageBridge,
    private val _analytics: GoogleAnalytics,
    private val _trackingId: String) {
    companion object {
        private const val kPrefix = "GoogleAnalytics"
    }

    private val _tracker = _analytics.newTracker(_trackingId)

    init {
        registerHandlers()
    }

    @AnyThread
    fun destroy() {
        deregisterHandlers()
    }

    private val kSetParameter: String
        @AnyThread get() = "${kPrefix}_setParameter_$_trackingId"

    private val kSetAllowIDFACollection: String
        @AnyThread get() = "${kPrefix}_setAllowIDFACollection_$_trackingId"

    private val kSend: String
        @AnyThread get() = "${kPrefix}_send_$_trackingId"

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kSetParameter) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: String
            )

            val request = deserialize<Request>(message)
            setParameter(request.key, request.value)
            ""
        }
        _bridge.registerHandler(kSetAllowIDFACollection) { message ->
            setAdvertisingIdCollectionEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSend) { message ->
            val params = deserializeMap<String, String>(message)
            send(params)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kSetParameter)
        _bridge.deregisterHandler(kSetAllowIDFACollection)
        _bridge.deregisterHandler(kSend)
    }

    @AnyThread
    fun setParameter(key: String, value: String) {
        Thread.runOnMainThread {
            _tracker[key] = value
        }
    }

    @AnyThread
    private fun setAdvertisingIdCollectionEnabled(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.enableAdvertisingIdCollection(enabled)
        }
    }

    @AnyThread
    fun setExceptionReportingEnabled(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.enableExceptionReporting(enabled)
        }
    }

    @AnyThread
    fun send(params: Map<String, String>) {
        Thread.runOnMainThread {
            _tracker.send(params)
        }
    }
}