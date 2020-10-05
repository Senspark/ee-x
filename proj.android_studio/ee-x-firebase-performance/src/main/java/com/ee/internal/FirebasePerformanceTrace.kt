package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.IMessageBridge
import com.google.firebase.perf.metrics.Trace
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable

@InternalSerializationApi
internal class FirebasePerformanceTrace(
    private val _bridge: IMessageBridge,
    private val _trace: Trace,
    private val _traceName: String) {
    init {
        registerHandlers()
    }

    fun destroy() {
        deregisterHandlers()
    }

    private val k__start: String
        @AnyThread get() = "FirebasePerformance_start_$_traceName"

    private val k__stop: String
        @AnyThread get() = "FirebasePerformance_stop_$_traceName"

    private val k__incrementMetric: String
        @AnyThread get() = "FirebasePerformance_incrementMetric_$_traceName"

    private val k__getLongMetric: String
        @AnyThread get() = "FirebasePerformance_getLongMetric_$_traceName"

    private val k__putMetric: String
        @AnyThread get() = "FirebasePerformance_putMetric_$_traceName"

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__start) {
            start()
            ""
        }
        _bridge.registerHandler(k__stop) {
            stop()
            ""
        }
        _bridge.registerHandler(k__incrementMetric) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: Long
            )

            val request = deserialize<Request>(message)
            incrementMetric(request.key, request.value)
            ""
        }
        _bridge.registerHandler(k__putMetric) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: Long
            )

            val request = deserialize<Request>(message)
            putMetric(request.key, request.value)
            ""
        }
        _bridge.registerHandler(k__getLongMetric) { message ->
            getLongMetric(message).toString()
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__start)
        _bridge.deregisterHandler(k__stop)
        _bridge.deregisterHandler(k__incrementMetric)
        _bridge.deregisterHandler(k__putMetric)
        _bridge.deregisterHandler(k__getLongMetric)
    }

    @AnyThread
    fun start() {
        _trace.start()
    }

    @AnyThread
    fun stop() {
        _trace.stop()
    }

    @AnyThread
    private fun incrementMetric(traceName: String, value: Long) {
        _trace.incrementMetric(traceName, value)
    }

    @AnyThread
    private fun putMetric(name: String, value: Long) {
        _trace.putMetric(name, value)
    }

    @AnyThread
    private fun getLongMetric(name: String): Long {
        return _trace.getLongMetric(name)
    }
}