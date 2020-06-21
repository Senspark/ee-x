package com.ee.appsflyer

import android.app.Activity
import android.app.Application
import android.content.Context
import android.content.Intent
import com.appsflyer.AppsFlyerConversionListener
import com.appsflyer.AppsFlyerLib
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.registerHandler
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

@ImplicitReflectionSerializer
@UnstableDefault
private class AppsFlyer(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(AppsFlyer::class.java.name)

        private const val kPrefix = "AppsFlyer"
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

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        return false
    }

    override fun destroy() {
        deregisterHandlers()
    }

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
        _bridge.registerHandler(kTrackEvent) { message ->
            @Serializable
            class Request(
                val name: String,
                val values: Map<String, Any>
            )

            val request = deserialize<Request>(message)
            trackEvent(request.name, request.values)
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kStartTracking)
        _bridge.deregisterHandler(kGetDeviceId)
        _bridge.deregisterHandler(kSetDebugEnabled)
        _bridge.deregisterHandler(kSetStopTracking)
        _bridge.deregisterHandler(kTrackEvent)
    }

    fun initialize(devKey: String) {
        Thread.checkMainThread()
        val listener = object : AppsFlyerConversionListener {
            override fun onConversionDataSuccess(conversionData: Map<String, Any>) {
                for (key in conversionData.keys) {
                    _logger.debug("${this::onConversionDataSuccess}: $key = ${conversionData[key]}")
                }
            }

            override fun onConversionDataFail(errorMessage: String) {
                _logger.debug("${this::onConversionDataFail}: $errorMessage")
            }

            override fun onAppOpenAttribution(conversionData: Map<String, String>) {
                for (key in conversionData.keys) {
                    _logger.debug("${this::onAppOpenAttribution}: $key = ${conversionData[key]}")
                }
            }

            override fun onAttributionFailure(errorMessage: String) {
                _logger.debug("${this::onAttributionFailure}: $errorMessage")
            }
        }
        _tracker.init(devKey, listener, _context)
    }

    fun startTracking() {
        _tracker.startTracking(_context as Application?)
    }

    val deviceId: String
        get() = _tracker.getAppsFlyerUID(_context)

    fun setDebugEnabled(enabled: Boolean) {
        _tracker.setDebugLog(enabled)
    }

    fun trackEvent(name: String, values: Map<String, Any>) {
        _tracker.trackEvent(_context, name, values)
    }
}