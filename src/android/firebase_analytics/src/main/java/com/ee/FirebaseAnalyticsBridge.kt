package com.ee

import android.app.Activity
import android.app.Application
import android.os.Bundle
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.google.common.truth.Truth.assertThat
import com.google.firebase.analytics.FirebaseAnalytics
import com.google.firebase.analytics.ktx.analytics
import com.google.firebase.analytics.ktx.logEvent
import com.google.firebase.ktx.Firebase
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.JsonPrimitive
import kotlinx.serialization.json.doubleOrNull
import kotlinx.serialization.json.longOrNull

@InternalSerializationApi
class FirebaseAnalyticsBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebaseAnalyticsBridge::class.java.name
        private const val kPrefix = "FirebaseAnalyticsBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetUserProperty = "${kPrefix}SetUserProperty"
        private const val kTrackScreen = "${kPrefix}TrackScreen"
        private const val kLogEvent = "${kPrefix}LogEvent"
    }

    private var _plugin: FirebaseAnalytics? = null

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) {
            Utils.toString(initialize())
        }
        _bridge.registerHandler(kSetUserProperty) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: String,
            )

            val request = deserialize<Request>(message)
            setUserProperty(request.key, request.value)
            ""
        }
        _bridge.registerHandler(kTrackScreen) { message ->
            trackScreen(message)
            ""
        }
        _bridge.registerHandler(kLogEvent) { message ->
            @Serializable
            class Request(
                val name: String,
                val parameters: Map<String, JsonPrimitive>
            )

            val request = deserialize<Request>(message)
            logEvent(request.name, request.parameters)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetUserProperty)
        _bridge.deregisterHandler(kLogEvent)
        _bridge.deregisterHandler(kTrackScreen)
    }

    @AnyThread
    private suspend fun initialize(): Boolean {
        if (FirebaseInitializer.instance.initialize(false)) {
            _plugin = Firebase.analytics
            return true
        }
        return false
    }

    @AnyThread
    private fun setUserProperty(key: String, value: String) {
        Thread.runOnMainThread {
            val plugin = _plugin ?: throw IllegalStateException("Please call initialize() first")
            plugin.setUserProperty(key, value)
        }
    }

    @AnyThread
    private fun trackScreen(name: String) {
        Thread.runOnMainThread {
            val plugin = _plugin ?: throw IllegalStateException("Please call initialize() first")
            plugin.logEvent(FirebaseAnalytics.Event.SCREEN_VIEW) {
                param(FirebaseAnalytics.Param.SCREEN_NAME, name)
            }
        }
    }

    @AnyThread
    private fun logEvent(name: String, parameters: Map<String, JsonPrimitive>) {
        Thread.runOnMainThread {
            val plugin = _plugin ?: throw IllegalStateException("Please call initialize() first")
            val bundle = Bundle()
            for ((key, value) in parameters) {
                if (value.isString) {
                    bundle.putString(key, value.content)
                    continue
                }
                val long = value.longOrNull
                if (long != null) {
                    bundle.putLong(key, long)
                    continue
                }
                val double = value.doubleOrNull
                if (double != null) {
                    bundle.putDouble(key, double)
                    continue
                }
                assertThat(false).isTrue()
            }
            plugin.logEvent(name, bundle)
        }
    }
}