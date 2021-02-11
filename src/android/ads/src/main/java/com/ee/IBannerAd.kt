package com.ee

import android.graphics.Point

interface IBannerAd : IAd {
    var isVisible: Boolean
    var position: Point
    var size: Point
}