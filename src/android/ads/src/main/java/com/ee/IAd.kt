package com.ee

import android.app.Activity
import androidx.annotation.AnyThread
import androidx.annotation.UiThread

interface IAd {
    @UiThread
    fun onCreate(activity: Activity)

    @UiThread
    fun onResume()

    @UiThread
    fun onPause()

    @UiThread
    fun onDestroy()

    @AnyThread
    fun destroy()
    
    val isLoaded: Boolean
    fun load()
}