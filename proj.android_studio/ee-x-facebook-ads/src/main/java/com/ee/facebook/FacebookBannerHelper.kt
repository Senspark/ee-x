package com.ee.facebook

import android.content.res.Resources
import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.core.internal.Utils
import com.facebook.ads.AdSize
import com.google.common.truth.Truth.assertThat

internal class FacebookBannerHelper {
    companion object {
        @AnyThread
        private fun getWidthInPixels(adSize: AdSize): Int {
            return when (adSize.width) {
                0, -1 -> Resources.getSystem().displayMetrics.widthPixels
                else -> Utils.convertDpToPixel(adSize.width.toDouble()).toInt()
            }
        }

        @AnyThread
        private fun getHeightInPixels(adSize: AdSize): Int {
            return when (adSize.height) {
                0 -> Resources.getSystem().displayMetrics.heightPixels
                else -> Utils.convertDpToPixel(adSize.height.toDouble()).toInt()
            }
        }

        @AnyThread
        private fun convertAdSizeToSize(adSize: AdSize): Point {
            val width = getWidthInPixels(adSize)
            val height = getHeightInPixels(adSize)
            return Point(width, height)
        }
    }

    private val _sizes: MutableMap<Int, Point> = HashMap()

    init {
        for (i in 0..4) {
            val adSize = getAdSize(i)
            val size = convertAdSizeToSize(adSize)
            _sizes[i] = size
        }
    }

    @AnyThread
    fun getAdSize(index: Int): AdSize {
        if (index == 0) {
            return AdSize.BANNER_HEIGHT_50
        }
        if (index == 1) {
            return AdSize.BANNER_HEIGHT_90
        }
        if (index == 2) {
            return AdSize.INTERSTITIAL
        }
        if (index == 3) {
            return AdSize.RECTANGLE_HEIGHT_250
        }
        if (index == 4) {
            return AdSize.BANNER_320_50
        }
        assertThat(false).isTrue()
        return AdSize.BANNER_320_50
    }

    @AnyThread
    fun getSize(index: Int): Point {
        return _sizes[index] ?: throw IllegalArgumentException("Invalid size index")
    }
}