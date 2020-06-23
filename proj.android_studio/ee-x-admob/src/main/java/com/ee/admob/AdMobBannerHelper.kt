package com.ee.admob

import android.content.Context
import android.graphics.Point
import androidx.annotation.AnyThread
import com.google.android.gms.ads.AdSize
import com.google.common.truth.Truth.assertThat

internal class AdMobBannerHelper(context: Context) {
    companion object {
        @AnyThread
        private fun convertAdSizeToSize(context: Context, adSize: AdSize): Point {
            val width = adSize.getWidthInPixels(context)
            val height = adSize.getHeightInPixels(context)
            return Point(width, height)
        }
    }

    private val _sizes: MutableMap<Int, Point> = HashMap()

    init {
        for (i in 0..3) {
            val adSize = getAdSize(i)
            val size = convertAdSizeToSize(context, adSize)
            _sizes[i] = size
        }
    }

    @AnyThread
    fun getAdSize(index: Int): AdSize {
        if (index == 0) {
            return AdSize.BANNER
        }
        if (index == 1) {
            return AdSize.LARGE_BANNER
        }
        if (index == 2) {
            return AdSize.SMART_BANNER
        }
        if (index == 3) {
            return AdSize.MEDIUM_RECTANGLE
        }
        assertThat(false).isTrue()
        return AdSize.INVALID
    }

    @AnyThread
    fun getSize(index: Int): Point {
        return _sizes[index] ?: throw IllegalArgumentException("Invalid size index")
    }
}