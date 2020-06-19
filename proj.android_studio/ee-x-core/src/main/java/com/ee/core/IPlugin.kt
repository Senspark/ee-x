package com.ee.core

import android.app.Activity
import android.content.Intent

/**
 * Created by Zinge on 6/1/16.
 */
interface IPlugin {
    /**
     * Called when the main activity has been created.
     */
    fun onCreate(activity: Activity)

    /**
     * Called when the main activity has been started.
     */
    fun onStart()

    /**
     * Called when the main activity has been stopped.
     */
    fun onStop()

    /**
     * Called when the main activity has been resumed.
     */
    fun onResume()

    /**
     * Called when the main activity has been paused.
     */
    fun onPause()

    /**
     * Called when the main activity has been destroyed.
     */
    fun onDestroy()

    /**
     * Called when the application has been destroyed.
     */
    fun destroy()

    fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean
}