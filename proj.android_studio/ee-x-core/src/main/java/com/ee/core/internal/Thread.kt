package com.ee.core.internal

import android.os.Handler
import android.os.Looper
import androidx.annotation.AnyThread
import com.ee.core.Logger
import com.google.common.truth.Truth.assertThat

object Thread {
    private val _logger = Logger(Thread::class.java.name)

    /// https://stackoverflow.com/questions/1069066/get-current-stack-trace-in-java
    private val currentStackTrace: Array<StackTraceElement>
        @AnyThread get() = java.lang.Thread.currentThread().stackTrace

    @AnyThread
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

    @AnyThread
    fun isMainThread(): Boolean {
        return java.lang.Thread.currentThread() === Looper.getMainLooper().thread
    }

    @AnyThread
    fun runOnMainThread(callback: Runnable): Boolean {
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

    @AnyThread
    @JvmStatic
    fun runOnMainThreadDelayed(seconds: Float, callback: Runnable) {
        val handler = Handler(Looper.getMainLooper())
        val result = handler.postDelayed(callback, seconds.toLong() * 1000)
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable")
        }
    }
}

private external fun ee_runOnMainThreadCallback()
private external fun ee_runOnMainThreadDelayedCallback(key: Int)

@Suppress("unused")
private fun ee_isMainThread(): Boolean {
    return Thread.isMainThread()
}

@Suppress("unused")
private fun ee_runOnMainThread(): Boolean {
    return Thread.runOnMainThread(Runnable {
        ee_runOnMainThreadCallback()
    })
}

@Suppress("unused")
private fun ee_runOnMainThreadDelayed(key: Int, delay: Float) {
    Thread.runOnMainThreadDelayed(delay, Runnable {
        ee_runOnMainThreadDelayedCallback(key)
    })
}