package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.ee.registerHandler
import com.google.android.gms.analytics.GoogleAnalytics
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 10/25/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
class GoogleAnalyticsTracker(
    private val _bridge: IMessageBridge,
    private val _analytics: GoogleAnalytics,
    private val _trackingId: String) {
    private val _tracker = _analytics.newTracker(_trackingId)

    init {
        registerHandlers()
    }

    @AnyThread
    fun destroy() {
        deregisterHandlers()
    }

    private val k__setParameter: String
        @AnyThread get() = "GoogleAnalytics_setParameter_$_trackingId"

    private val k__setAllowIDFACollection: String
        @AnyThread get() = "GoogleAnalytics_setAllowIDFACollection_$_trackingId"

    private val k__send: String
        @AnyThread get() = "GoogleAnalytics_send_$_trackingId"

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__setParameter) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: String
            )

            val request = deserialize<Request>(message)
            setParameter(request.key, request.value)
            ""
        }
        _bridge.registerHandler(k__setAllowIDFACollection) { message ->
            setAdvertisingIdCollectionEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__send) { message ->
            val params = deserializeMap<String, String>(message)
            send(params)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__setParameter)
        _bridge.deregisterHandler(k__setAllowIDFACollection)
        _bridge.deregisterHandler(k__send)
    }

    @AnyThread
    fun setParameter(key: String, value: String) {
        Thread.runOnMainThread(Runnable {
            _tracker[key] = value
        })
    }

    @AnyThread
    private fun setAdvertisingIdCollectionEnabled(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            _tracker.enableAdvertisingIdCollection(enabled)
        })
    }

    @AnyThread
    fun setExceptionReportingEnabled(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            _tracker.enableExceptionReporting(enabled)
        })
    }

    @AnyThread
    fun send(params: Map<String, String>) {
        Thread.runOnMainThread(Runnable {
            _tracker.send(params)
        })
    }
}