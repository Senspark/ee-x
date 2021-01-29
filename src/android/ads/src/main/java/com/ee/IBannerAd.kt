package com.ee

import android.graphics.Point

interface IBannerAd : IAd {
    var position: Point
    var size: Point
    var isVisible: Boolean
}