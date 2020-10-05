package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.appsflyer.AppsFlyerConversionListener
import com.appsflyer.AppsFlyerLib
import com.ee.internal.deserialize
import kotlinx.serialization.Contextual
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable

@InternalSerializationApi
class AppsFlyerBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(AppsFlyerBridge::class.java.name)

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
        _logger.debug("constructor begin: context = $_context")
        registerHandlers()
        _logger.debug("constructor end.")
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            initialize(message)
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
            @Serializable
            class Request(
                val name: String,
                val values: Map<String, @Contextual Any>
            )

            val request = deserialize<Request>(message)
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
                        _logger.debug("${this::onConversionDataSuccess.name}: $key = ${conversionData[key]}")
                    }
                }

                override fun onConversionDataFail(errorMessage: String) {
                    _logger.debug("${this::onConversionDataFail.name}: $errorMessage")
                }

                override fun onAppOpenAttribution(conversionData: Map<String, String>) {
                    for (key in conversionData.keys) {
                        _logger.debug("${this::onAppOpenAttribution.name}: $key = ${conversionData[key]}")
                    }
                }

                override fun onAttributionFailure(errorMessage: String) {
                    _logger.debug("${this::onAttributionFailure.name}: $errorMessage")
                }
            }
            _tracker.init(devKey, listener, _context)
            _tracker.setDeviceTrackingDisabled(true)
            _tracker.enableLocationCollection(true)
        }
    }

    @AnyThread
    fun startTracking() {
        Thread.runOnMainThread {
            _tracker.startTracking(_context)
        }
    }

    val deviceId: String
        @AnyThread get() = _tracker.getAppsFlyerUID(_context)

    @AnyThread
    fun setDebugEnabled(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.setDebugLog(enabled)
        }
    }

    @AnyThread
    fun setStopTracking(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.stopTracking(enabled, _context)
        }
    }

    @AnyThread
    fun trackEvent(name: String, values: Map<String, Any>) {
        Thread.runOnMainThread {
            _tracker.trackEvent(_context, name, values)
        }
    }
}