package com.ee.internal

import android.os.Handler
import android.os.Looper
import androidx.annotation.AnyThread
import com.ee.ILogger
import com.google.common.truth.Truth

class ThreadImpl(private val _logger: ILogger)
    : IThreadImpl {
    companion object {
        private val kTag = ThreadImpl::class.java.name
    }

    /// https://stackoverflow.com/questions/1069066/get-current-stack-trace-in-java
    private val currentStackTrace: Array<StackTraceElement>
        @AnyThread get() = Thread.currentThread().stackTrace

    override fun checkMainThread() {
        if (!isMainThread()) {
            _logger.error("$kTag: Current thread is not the main thread")
            for (e in currentStackTrace) {
                _logger.warn(e.toString())
            }
            Truth.assertThat(false).isTrue()
        }
    }

    override fun isMainThread(): Boolean {
        return Thread.currentThread() === Looper.getMainLooper().thread
    }

    override fun runOnMainThread(callback: Runnable): Boolean {
        if (isMainThread()) {
            callback.run()
            return true
        }
        val handler = Handler(Looper.getMainLooper())
        val result = handler.post(callback)
        if (!result) {
            _logger.error("$kTag: runOnUiThread: failed to post the runnable")
        }
        return false
    }

    override fun runOnMainThreadDelayed(seconds: Float, callback: Runnable) {
        val handler = Handler(Looper.getMainLooper())
        val result = handler.postDelayed(callback, seconds.toLong() * 1000)
        if (!result) {
            _logger.error("$kTag: runOnUiThread: failed to post the runnable")
        }
    }
}