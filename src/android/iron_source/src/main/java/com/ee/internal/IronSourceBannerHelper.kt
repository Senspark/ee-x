package com.ee.internal

import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.Platform
import com.ironsource.mediationsdk.ISBannerSize

class IronSourceBannerHelper {
    companion object {
        @AnyThread
        private fun convertAdSizeToSize(adSize: ISBannerSize): Point {
            val density = Platform.getDensity()
            val width = adSize.width * density
            val height = adSize.height * density
            return Point(width.toInt(), height.toInt())
        }
    }

    private val _indexToSize: MutableMap<Int, Point> = HashMap()

    init {
        for (index in 0..3) {
            val adSize = getAdSize(index)
            val size = convertAdSizeToSize(adSize)
            _indexToSize[index] = size
        }
    }

    @AnyThread
    fun getAdSize(index: Int): ISBannerSize {
        if (index == 0) {
            return ISBannerSize.BANNER
        }
        if (index == 1) {
            return ISBannerSize.LARGE
        }
        if (index == 2) {
            return ISBannerSize.RECTANGLE
        }
        throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    private fun getIndex(adSize: ISBannerSize): Int {
        if (adSize == ISBannerSize.BANNER) {
            return 0
        }
        if (adSize == ISBannerSize.LARGE) {
            return 1
        }
        if (adSize == ISBannerSize.RECTANGLE) {
            return 2
        }
        throw IllegalArgumentException("Invalid ad size")
    }

    @AnyThread
    fun getSize(index: Int): Point {
        return _indexToSize[index] ?: throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    fun getSize(adSize: ISBannerSize): Point {
        return getSize(getIndex(adSize))
    }
}