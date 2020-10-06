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
    companion object {
        private const val kPrefix = "FirebasePerformance"
    }

    init {
        registerHandlers()
    }

    fun destroy() {
        deregisterHandlers()
    }

    private val kStart: String
        @AnyThread get() = "${kPrefix}_start_$_traceName"

    private val kStop: String
        @AnyThread get() = "${kPrefix}_stop_$_traceName"

    private val kIncrementMetric: String
        @AnyThread get() = "${kPrefix}_incrementMetric_$_traceName"

    private val kGetLongMetric: String
        @AnyThread get() = "${kPrefix}_getLongMetric_$_traceName"

    private val kPutMetric: String
        @AnyThread get() = "${kPrefix}_putMetric_$_traceName"

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kStart) {
            start()
            ""
        }
        _bridge.registerHandler(kStop) {
            stop()
            ""
        }
        _bridge.registerHandler(kIncrementMetric) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: Long
            )

            val request = deserialize<Request>(message)
            incrementMetric(request.key, request.value)
            ""
        }
        _bridge.registerHandler(kPutMetric) { message ->
            @Serializable
            class Request(
                val key: String,
                val value: Long
            )

            val request = deserialize<Request>(message)
            putMetric(request.key, request.value)
            ""
        }
        _bridge.registerHandler(kGetLongMetric) { message ->
            getLongMetric(message).toString()
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kStart)
        _bridge.deregisterHandler(kStop)
        _bridge.deregisterHandler(kIncrementMetric)
        _bridge.deregisterHandler(kPutMetric)
        _bridge.deregisterHandler(kGetLongMetric)
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