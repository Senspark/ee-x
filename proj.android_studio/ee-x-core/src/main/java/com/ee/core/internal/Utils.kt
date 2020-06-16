package com.ee.core.internal

import android.app.Activity
import android.content.Context
import android.os.Handler
import android.os.Looper
import android.view.View
import android.widget.FrameLayout
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.registerAsyncHandler
import com.ee.core.registerHandler
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 10/9/17.
 */
object Utils {
    private val _logger = Logger(Utils::class.java.name)
    private const val kPrefix = "Utils_"
    private const val kIsMainThread = kPrefix + "isMainThread"
    private const val kRunOnUiThread = kPrefix + "runOnUiThread"
    private const val kRunOnUiThreadDelayed = kPrefix + "runOnUiThreadDelayed"
    private const val kRunOnUiThreadCallback = kPrefix + "runOnUiThreadCallback"

    @JvmStatic
    fun getRootView(activity: Activity): FrameLayout {
        return activity.findViewById<View>(android.R.id.content).rootView as FrameLayout
    }

    @JvmStatic
    fun toString(value: Boolean): String {
        return if (value) "true" else "false"
    }

    @JvmStatic
    fun toBoolean(value: String): Boolean {
        assertThat(value).isAnyOf("true", "false")
        return "true" == value
    }

    /// https://stackoverflow.com/questions/1069066/get-current-stack-trace-in-java
    private val currentStackTrace: Array<StackTraceElement>
        get() = Thread.currentThread().stackTrace

    @ImplicitReflectionSerializer
    @UnstableDefault
    fun registerHandlers(bridge: IMessageBridge, context: Context) {
        bridge.registerHandler(kIsMainThread) {
            toString(isMainThread())
        }
        bridge.registerHandler(kRunOnUiThread) {
            toString(runOnUiThread(Runnable {
                bridge.callCpp(kRunOnUiThreadCallback)
            }))
        }
        bridge.registerAsyncHandler(kRunOnUiThreadDelayed) { message, resolver ->
            @Serializable
            class Request(
                val delay: Float
            )

            val request = deserialize<Request>(message)
            runOnUiThreadDelayed(request.delay, Runnable {
                resolver.resolve("")
            })
        }
    }

    @JvmStatic
    fun deregisterHandlers(bridge: IMessageBridge) {
        bridge.deregisterHandler(kIsMainThread)
        bridge.deregisterHandler(kRunOnUiThread)
        bridge.deregisterHandler(kRunOnUiThreadDelayed)
    }

    @JvmStatic
    fun checkMainThread() {
        if (!isMainThread()) {
            _logger.error("Current thread is not the main thread")
            for (e in currentStackTrace) {
                _logger.warn(e.toString())
            }
            assertThat(false).isTrue()
        }
    }

    fun isMainThread(): Boolean {
        return Thread.currentThread() === Looper.getMainLooper().thread
    }

    fun runOnUiThread(callback: Runnable): Boolean {
        if (isMainThread()) {
            callback.run()
            return true
        }
        val handler = Handler(Looper.getMainLooper())
        val result = handler.post(callback)
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable")
        }
        return false
    }

    @JvmStatic
    fun runOnUiThreadDelayed(seconds: Float, callback: Runnable) {
        val handler = Handler(Looper.getMainLooper())
        val result = handler.postDelayed(callback, seconds.toLong() * 1000)
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable")
        }
    }

    /// https://stackoverflow.com/questions/4605527/converting-pixels-to-dp
    @JvmStatic
    fun convertDpToPixel(dp: Double): Double {
        return dp * Platform.getDensity()
    }
}