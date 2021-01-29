package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.FirebasePerformanceTrace
import com.google.firebase.ktx.Firebase
import com.google.firebase.perf.ktx.performance
import kotlinx.serialization.InternalSerializationApi
import java.util.concurrent.ConcurrentHashMap

@InternalSerializationApi
class FirebasePerformanceBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebasePerformanceBridge::class.java.name
        private const val kPrefix = "FirebasePerformanceBridge"
        private const val kSetDataCollectionEnabled = "${kPrefix}SetDataCollectionEnabled"
        private const val kIsDataCollectionEnabled = "${kPrefix}IsDataCollectionEnabled"
        private const val kNewTrace = "${kPrefix}NewTrace"
    }

    private val _performance = Firebase.performance
    private val _traces: MutableMap<String, FirebasePerformanceTrace> = ConcurrentHashMap()

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end.")
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
        for (trace in _traces.values) {
            trace.destroy()
        }
        _traces.clear()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kSetDataCollectionEnabled) { message ->
            isDataCollectionEnabled = Utils.toBoolean(message)
            ""
        }
        _bridge.registerHandler(kIsDataCollectionEnabled) {
            Utils.toString(isDataCollectionEnabled)
        }
        _bridge.registerHandler(kNewTrace) { message ->
            Utils.toString(newTrace(message))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kSetDataCollectionEnabled)
        _bridge.deregisterHandler(kIsDataCollectionEnabled)
        _bridge.deregisterHandler(kNewTrace)
    }

    private var isDataCollectionEnabled: Boolean
        @AnyThread get() = _performance.isPerformanceCollectionEnabled
        @AnyThread set(value) {
            _performance.isPerformanceCollectionEnabled = value
        }

    @AnyThread
    fun newTrace(traceName: String): Boolean {
        if (_traces.containsKey(traceName)) {
            return false
        }
        val trace = _performance.newTrace(traceName)
        _traces[traceName] = FirebasePerformanceTrace(_bridge, trace, traceName)
        return true
    }
}