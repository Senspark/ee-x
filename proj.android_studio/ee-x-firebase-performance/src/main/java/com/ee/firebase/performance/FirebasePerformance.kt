package com.ee.firebase.performance

import android.app.Activity
import android.content.Context
import android.content.Intent
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import java.util.concurrent.ConcurrentHashMap

@ImplicitReflectionSerializer
@UnstableDefault
class FirebasePerformance(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(FirebasePerformance::class.java.name)

        private const val k__setDataCollectionEnabled = "FirebasePerformance_setDataCollectionEnabled"
        private const val k__isDataCollectionEnabled = "FirebasePerformance_isDataCollectionEnabled"
        private const val k__startTrace = "FirebasePerformance_startTrace"
        private const val k__newTrace = "FirebasePerformance_newTrace"
    }

    private val _performance = com.google.firebase.perf.FirebasePerformance.getInstance()
    private val _traces: MutableMap<String, FirebasePerformanceTrace> = ConcurrentHashMap()

    init {
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
        for (trace in _traces.values) {
            trace.destroy()
        }
        _traces.clear()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__setDataCollectionEnabled) { message ->
            isDataCollectionEnabled = Utils.toBoolean(message)
            ""
        }
        _bridge.registerHandler(k__isDataCollectionEnabled) {
            Utils.toString(isDataCollectionEnabled)
        }
        _bridge.registerHandler(k__startTrace) { message ->
            Utils.toString(startTrace(message))
        }
        _bridge.registerHandler(k__newTrace) { message ->
            Utils.toString(newTrace(message))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__setDataCollectionEnabled)
        _bridge.deregisterHandler(k__isDataCollectionEnabled)
        _bridge.deregisterHandler(k__newTrace)
        _bridge.deregisterHandler(k__startTrace)
    }

    private var isDataCollectionEnabled: Boolean
        @AnyThread get() = _performance.isPerformanceCollectionEnabled
        @AnyThread set(value) {
            _performance.isPerformanceCollectionEnabled = value
        }

    @AnyThread
    fun startTrace(traceName: String): Boolean {
        if (_traces.containsKey(traceName)) {
            return false
        }
        val trace = com.google.firebase.perf.FirebasePerformance.startTrace(traceName)
        _traces[traceName] = FirebasePerformanceTrace(_bridge, trace, traceName)
        return true
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