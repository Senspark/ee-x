package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.appsflyer.AppsFlyerConversionListener
import com.appsflyer.AppsFlyerLib
import com.ee.internal.deserialize
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.JsonPrimitive
import kotlinx.serialization.json.doubleOrNull
import kotlinx.serialization.json.longOrNull

class AppsFlyerBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = AppsFlyerBridge::class.java.name
        private const val kPrefix = "AppsFlyerBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kStartTracking = "${kPrefix}StartTracking"
        private const val kGetDeviceId = "${kPrefix}GetDeviceId"
        private const val kSetDebugEnabled = "${kPrefix}SetDebugEnabled"
        private const val kSetStopTracking = "${kPrefix}SetStopTracking"
        private const val kTrackEvent = "${kPrefix}TrackEvent"
    }

    private val _tracker = AppsFlyerLib.getInstance()

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

    @Serializable
    private class InitializeRequest(
        val devKey: String,
        val iosAppId: String
    )

    @Serializable
    private class TrackEventRequest(
        val name: String,
        val values: Map<String, JsonPrimitive>
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            val request = deserialize<InitializeRequest>(message)
            initialize(request.devKey)
            ""
        }
        _bridge.registerHandler(kStartTracking) {
            startTracking()
            ""
        }
        _bridge.registerHandler(kGetDeviceId) {
            deviceId
        }
        _bridge.registerHandler(kSetDebugEnabled) { message ->
            setDebugEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetStopTracking) { message ->
            setStopTracking(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kTrackEvent) { message ->
            val request = deserialize<TrackEventRequest>(message)
            trackEvent(request.name, request.values)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kStartTracking)
        _bridge.deregisterHandler(kGetDeviceId)
        _bridge.deregisterHandler(kSetDebugEnabled)
        _bridge.deregisterHandler(kSetStopTracking)
        _bridge.deregisterHandler(kTrackEvent)
    }

    @AnyThread
    fun initialize(devKey: String) {
        Thread.runOnMainThread {
            val listener = object : AppsFlyerConversionListener {
                override fun onConversionDataSuccess(conversionData: Map<String, Any>) {
                    for (key in conversionData.keys) {
                        _logger.debug("$kTag: ${this::onConversionDataSuccess.name}: $key = ${conversionData[key]}")
                    }
                }

                override fun onConversionDataFail(errorMessage: String) {
                    _logger.debug("$kTag: ${this::onConversionDataFail.name}: $errorMessage")
                }

                override fun onAppOpenAttribution(conversionData: Map<String, String>) {
                    for (key in conversionData.keys) {
                        _logger.debug("$kTag: ${this::onAppOpenAttribution.name}: $key = ${conversionData[key]}")
                    }
                }

                override fun onAttributionFailure(errorMessage: String) {
                    _logger.debug("$kTag: ${this::onAttributionFailure.name}: $errorMessage")
                }
            }
            _tracker.init(devKey, listener, _application)
            _tracker.anonymizeUser(false)
        }
    }

    @AnyThread
    fun startTracking() {
        Thread.runOnMainThread {
            _tracker.start(_application)
        }
    }

    val deviceId: String
        @AnyThread get() = _tracker.getAppsFlyerUID(_application)!!

    @AnyThread
    fun setDebugEnabled(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.setDebugLog(enabled)
        }
    }

    @AnyThread
    fun setStopTracking(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.stop(enabled, _application)
        }
    }

    @AnyThread
    fun trackEvent(name: String, values: Map<String, JsonPrimitive>) {
        Thread.runOnMainThread {
            val parsedValues = HashMap<String, Any>()
            for ((key, value) in values) {
                if (value.isString) {
                    parsedValues[key] = value.content
                    continue
                }
                val long = value.longOrNull
                if (long != null) {
                    parsedValues[key] = long
                    continue
                }
                val double = value.doubleOrNull
                if (double != null) {
                    parsedValues[key] = double
                    continue
                }
                assert(false)
            }
            _tracker.logEvent(_application, name, values)
        }
    }
}