package com.ee

import androidx.annotation.AnyThread
import com.ee.internal.IThreadImpl

object Thread {
    private var _impl: IThreadImpl? = null

    @AnyThread
    fun registerImpl(impl: IThreadImpl) {
        _impl = impl
    }

    @AnyThread
    @JvmStatic
    fun checkMainThread() {
        _impl?.checkMainThread()
    }

    @AnyThread
    @JvmStatic // Used by UnityThread.
    fun isMainThread(): Boolean {
        return _impl?.isMainThread() ?: false
    }

    @AnyThread
    @JvmStatic // Used by UnityThread.
    fun runOnMainThread(callback: Runnable): Boolean {
        return _impl?.runOnMainThread(callback) ?: false
    }

    @AnyThread
    @JvmStatic
    fun runOnMainThreadDelayed(seconds: Float, callback: Runnable) {
        _impl?.runOnMainThreadDelayed(seconds, callback)
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
    return Thread.runOnMainThread {
        ee_runOnMainThreadCallback()
    }
}

@Suppress("unused")
private fun ee_runOnMainThreadDelayed(key: Int, delay: Float) {
    Thread.runOnMainThreadDelayed(delay) {
        ee_runOnMainThreadDelayedCallback(key)
    }
}