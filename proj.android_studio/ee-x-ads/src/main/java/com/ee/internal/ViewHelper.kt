package com.ee.internal

import android.graphics.Point
import android.view.View
import android.widget.FrameLayout
import androidx.annotation.AnyThread
import com.ee.Thread
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicReference

class ViewHelper(private val _view: View) {
    private val _position = AtomicReference<Point>(Point(0, 0))
    private val _size = AtomicReference<Point>(Point(0, 0))
    private val _visible = AtomicBoolean(false)

    var position: Point
        @AnyThread get() = _position.get()
        @AnyThread set(value) {
            Thread.runOnMainThread(Runnable {
                val params = _view.layoutParams as FrameLayout.LayoutParams
                params.leftMargin = value.x
                params.topMargin = value.y
                _view.layoutParams = params
                _position.set(value)
            })
        }

    var size: Point
        @AnyThread get() = _size.get()
        @AnyThread set(value) {
            Thread.runOnMainThread(Runnable {
                val params = _view.layoutParams as FrameLayout.LayoutParams
                params.width = value.x
                params.height = value.y
                _view.layoutParams = params
                _size.set(value)
            })
        }

    var isVisible: Boolean
        @AnyThread get() = _visible.get()
        @AnyThread set(value) {
            Thread.runOnMainThread(Runnable {
                if (value) {
                    _view.visibility = View.VISIBLE
                } else {
                    // View.INVISIBLE doesn't trigger a view redraw.
                    // Production: load and then call setVisible(true) doesn't show the native ad.
                    _view.visibility = View.GONE
                }
                _visible.set(value)
            })
        }
}