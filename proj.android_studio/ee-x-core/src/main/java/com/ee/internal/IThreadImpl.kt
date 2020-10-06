package com.ee.internal

import androidx.annotation.AnyThread

interface IThreadImpl {
    @AnyThread
    fun checkMainThread()

    @AnyThread
    fun isMainThread(): Boolean

    @AnyThread
    fun runOnMainThread(callback: Runnable): Boolean

    @AnyThread
    fun runOnMainThreadDelayed(seconds: Float, callback: Runnable)
}