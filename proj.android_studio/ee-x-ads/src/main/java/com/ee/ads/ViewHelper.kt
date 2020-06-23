package com.ee.ads

import android.graphics.Point
import android.view.View
import android.widget.FrameLayout
import androidx.annotation.UiThread
import com.ee.core.internal.Thread

class ViewHelper(private val _view: View) {
    var position: Point
        @UiThread get() {
            Thread.checkMainThread()
            /*
            int p[] = new int[2];
            view.getLocationInWindow(p);
            return new Point(p[0], p[1]);
            */
            val params = _view.layoutParams as FrameLayout.LayoutParams
            return Point(params.leftMargin, params.topMargin)
        }
        @UiThread set(value) {
            Thread.checkMainThread()
            val params = _view.layoutParams as FrameLayout.LayoutParams
            params.leftMargin = value.x
            params.topMargin = value.y
            _view.layoutParams = params
        }

    var size: Point
        @UiThread get() {
            Thread.checkMainThread()
            // return new Point(view.getWidth(), view.getHeight());
            val params = _view.layoutParams as FrameLayout.LayoutParams
            return Point(params.width, params.height)
        }
        @UiThread set(value) {
            Thread.checkMainThread()
            val params = _view.layoutParams as FrameLayout.LayoutParams
            params.width = value.x
            params.height = value.y
            _view.layoutParams = params
        }

    var isVisible: Boolean
        @UiThread get() {
            Thread.checkMainThread()
            return _view.visibility == View.VISIBLE
        }
        @UiThread set(value) {
            Thread.checkMainThread()
            if (value) {
                _view.visibility = View.VISIBLE
            } else {
                // View.INVISIBLE doesn't trigger a view redraw.
                // Production: load and then call setVisible(true) doesn't show the native ad.
                _view.visibility = View.GONE
            }
        }
}