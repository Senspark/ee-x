package com.ee

import android.app.Activity
import android.app.Application
import android.content.res.Configuration
import android.view.View
import android.widget.FrameLayout
import androidx.annotation.AnyThread
import androidx.annotation.UiThread

/**
 * Created by Zinge on 10/9/17.
 */
object Utils {
    @JvmStatic
    @AnyThread
    fun getCurrentActivity(): Activity? {
        // https://stackoverflow.com/a/28423385
        val activityThreadClass = Class.forName("android.app.ActivityThread")
        val activityThread = activityThreadClass.getMethod("currentActivityThread").invoke(null)
        val activitiesField = activityThreadClass.getDeclaredField("mActivities")
        activitiesField.isAccessible = true

        val activities = activitiesField.get(activityThread) as? Map<*, *> ?: return null
        for (item in activities.values) {
            val activityRecord = item as Any
            val activityRecordClass = activityRecord::class.java
            val pausedField = activityRecordClass.getDeclaredField("paused")
            pausedField.isAccessible = true
            if (pausedField.getBoolean(activityRecord)) {
                // Ignored.
            } else {
                val activityField = activityRecordClass.getDeclaredField("activity")
                activityField.isAccessible = true
                @Suppress("UnnecessaryVariable")
                val activity = activityField.get(activityRecord) as Activity
                return activity
            }
        }
        return null
    }

    @JvmStatic
    @UiThread
    fun getRootView(activity: Activity): FrameLayout {
        return activity.findViewById<View>(android.R.id.content).rootView as FrameLayout
    }

    fun isLandscape(application: Application): Boolean {
        return application.resources.configuration.orientation == Configuration.ORIENTATION_LANDSCAPE
    }

    @AnyThread
    @JvmStatic
    fun toString(value: Boolean): String {
        return if (value) "true" else "false"
    }

    @AnyThread
    @JvmStatic
    fun toBoolean(value: String): Boolean {
        assert(value == "true" || value == "false")
        return "true" == value
    }

    /// https://stackoverflow.com/questions/4605527/converting-pixels-to-dp
    @AnyThread
    @JvmStatic
    fun convertDpToPixel(dp: Double): Double {
        return dp * Platform.getDensity()
    }
}