package com.ee.internal

import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.Utils
import com.unity3d.services.banners.UnityBannerSize

class UnityBannerHelper {
    companion object {
        @AnyThread
        private fun convertAdSizeToSize(adSize: UnityBannerSize): Point {
            return Point(
                Utils.convertDpToPixel(adSize.width.toDouble()).toInt(),
                Utils.convertDpToPixel(adSize.height.toDouble()).toInt())
        }
    }

    private val _indexToSize: MutableMap<Int, Point> = HashMap()

    init {
        for (index in 0..0) {
            val adSize = getAdSize(index)
            val size = convertAdSizeToSize(adSize)
            _indexToSize[index] = size
        }
    }

    @AnyThread
    fun getAdSize(index: Int): UnityBannerSize {
        if (index == 0) {
            return UnityBannerSize(320, 50)
        }
        throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    private fun getIndex(adSize: UnityBannerSize): Int {
        if (adSize.width == 320 && adSize.height == 50) {
            return 0
        }
        throw IllegalArgumentException("Invalid ad size")
    }

    @AnyThread
    fun getSize(index: Int): Point {
        return _indexToSize[index] ?: throw IllegalArgumentException("Invalid ad index")
    }

    @AnyThread
    fun getSize(adSize: UnityBannerSize): Point {
        return getSize(getIndex(adSize))
    }
}