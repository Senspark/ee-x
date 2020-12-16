package com.ee

import android.graphics.Point

interface IAdView {
    val isLoaded: Boolean
    fun load()
    var position: Point
    var size: Point
    var isVisible: Boolean
}