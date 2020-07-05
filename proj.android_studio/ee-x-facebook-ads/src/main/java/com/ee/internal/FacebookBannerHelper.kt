package com.ee.internal

import android.content.res.Resources
import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.Utils
import com.facebook.ads.AdSize

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

    private val _indexToSize: MutableMap<Int, Point> = HashMap()

    init {
        for (index in 0..4) {
            val adSize = getAdSize(index)
            val size = convertAdSizeToSize(adSize)
            _indexToSize[index] = size
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
        throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    private fun getIndex(adSize: AdSize): Int {
        if (adSize == AdSize.BANNER_HEIGHT_50) {
            return 0
        }
        if (adSize == AdSize.BANNER_HEIGHT_90) {
            return 1
        }
        if (adSize == AdSize.INTERSTITIAL) {
            return 2
        }
        if (adSize == AdSize.RECTANGLE_HEIGHT_250) {
            return 3
        }
        if (adSize == AdSize.BANNER_320_50) {
            return 4
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