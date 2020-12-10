package com.ee.internal

import android.content.Context
import android.graphics.Point
import androidx.annotation.AnyThread
import com.google.android.gms.ads.AdSize

internal class AdMobBannerHelper(context: Context) {
    companion object {
        @AnyThread
        private fun convertAdSizeToSize(context: Context, adSize: AdSize): Point {
            val width = adSize.getWidthInPixels(context)
            val height = adSize.getHeightInPixels(context)
            return Point(width, height)
        }
    }

    private val _indexToSize: MutableMap<Int, Point> = HashMap()

    init {
        for (index in 0..3) {
            val adSize = getAdSize(index)
            val size = convertAdSizeToSize(context, adSize)
            _indexToSize[index] = size
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
        throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    private fun getIndex(adSize: AdSize): Int {
        if (adSize == AdSize.BANNER) {
            return 0
        }
        if (adSize == AdSize.LARGE_BANNER) {
            return 1
        }
        if (adSize == AdSize.SMART_BANNER) {
            return 2
        }
        if (adSize == AdSize.MEDIUM_RECTANGLE) {
            return 3
        }
        throw IllegalArgumentException("Invalid ad size")
    }

    @AnyThread
    fun getSize(index: Int): Point {
        return _indexToSize[index] ?: throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    fun getSize(adSize: AdSize): Point {
        return getSize(getIndex(adSize))
    }
}