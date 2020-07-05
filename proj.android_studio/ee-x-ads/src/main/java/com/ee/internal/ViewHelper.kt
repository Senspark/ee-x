package com.ee.internal

import android.graphics.Color
import android.graphics.Point
import android.view.View
import android.widget.FrameLayout
import androidx.annotation.AnyThread
import com.ee.Thread
import java.util.concurrent.atomic.AtomicBoolean
import java.util.concurrent.atomic.AtomicReference

class ViewHelper(
    initialPosition: Point,
    initialSize: Point,
    initialVisible: Boolean) {
    companion object {
        private fun applyPosition(view: View, value: Point) {
            Thread.runOnMainThread(Runnable {
                val params = view.layoutParams as FrameLayout.LayoutParams
                params.leftMargin = value.x
                params.topMargin = value.y
                view.layoutParams = params
            })
        }

        private fun applySize(view: View, value: Point) {
            Thread.runOnMainThread(Runnable {
                val params = view.layoutParams as FrameLayout.LayoutParams
                params.width = value.x
                params.height = value.y
                view.layoutParams = params
            })
        }

        private fun applyVisible(view: View, value: Boolean) {
            Thread.runOnMainThread(Runnable {
                if (value) {
                    view.visibility = View.VISIBLE
                    // https://stackoverflow.com/questions/21408178/admob-wont-show-the-banner-until
                    // -refresh-or-sign-in-to-google-plus
                    view.setBackgroundColor(Color.BLACK)
                } else {
                    // View.INVISIBLE doesn't trigger a view redraw.
                    // Production: load and then call setVisible(true) doesn't show the native ad.
                    view.visibility = View.GONE
                }
            })
        }
    }

    private val _view = AtomicReference<View?>(null)
    private val _position = AtomicReference<Point>(Point(0, 0))
    private val _size = AtomicReference<Point>(Point(0, 0))
    private val _visible = AtomicBoolean(false)

    var view: View?
        @AnyThread get() = _view.get()
        @AnyThread set(value) {
            _view.set(value)
            value?.let { view ->
                applyPosition(view, position)
                applySize(view, size)
                applyVisible(view, isVisible)
            }
        }

    var position: Point
        @AnyThread get() = _position.get()
        @AnyThread set(value) {
            _position.set(value)
            view?.let { view ->
                applyPosition(view, value)
            }
        }

    var size: Point
        @AnyThread get() = _size.get()
        @AnyThread set(value) {
            _size.set(value)
            view?.let { view ->
                applySize(view, value)
            }
        }

    var isVisible: Boolean
        @AnyThread get() = _visible.get()
        @AnyThread set(value) {
            _visible.set(value)
            view?.let { view ->
                applyVisible(view, value)
            }
        }

    init {
        position = initialPosition
        size = initialSize
        isVisible = initialVisible
    }
}