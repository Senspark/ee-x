package com.ee.core.internal

import android.app.Activity
import android.view.View
import android.widget.FrameLayout
import com.ee.core.Logger
import com.google.common.truth.Truth.assertThat

/**
 * Created by Zinge on 10/9/17.
 */
object Utils {
    private val _logger = Logger(Utils::class.java.name)

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

    /// https://stackoverflow.com/questions/4605527/converting-pixels-to-dp
    @JvmStatic
    fun convertDpToPixel(dp: Double): Double {
        return dp * Platform.getDensity()
    }
}