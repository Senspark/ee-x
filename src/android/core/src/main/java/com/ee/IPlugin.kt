package com.ee

import android.app.Activity
import androidx.annotation.AnyThread
import androidx.annotation.UiThread

/**
 * Created by Zinge on 6/1/16.
 */
interface IPlugin {
    /**
     * Called when the main activity has been created.
     */
    @UiThread
    fun onCreate(activity: Activity)

    /**
     * Called when the main activity has been started.
     */
    @UiThread
    fun onStart()

    /**
     * Called when the main activity has been stopped.
     */
    @UiThread
    fun onStop()

    /**
     * Called when the main activity has been resumed.
     */
    @UiThread
    fun onResume()

    /**
     * Called when the main activity has been paused.
     */
    @UiThread
    fun onPause()

    /**
     * Called when the main activity has been destroyed.
     */
    @UiThread
    fun onDestroy()

    /**
     * Called when the application has been destroyed.
     */
    @AnyThread
    fun destroy()
}