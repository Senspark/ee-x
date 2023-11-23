package com.ee.cheat

import android.app.ActionBar.LayoutParams
import android.app.Activity
import android.util.Log
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import com.ee.CommandReceiverBridge
import com.google.android.material.floatingactionbutton.FloatingActionButton
import kotlin.math.abs

class Fab(
    private val activity: Activity,
    parentView: View
) :
    FloatingActionButton(activity),
    View.OnTouchListener {

    companion object {
        private const val kSize = 48
        private const val CLICK_DRAG_TOLERANCE =
            10f // Often, there will be a slight, unintentional, drag when the user taps the FAB, so we need to account for this.
    }

    private var downRawX = 0f
    private var downRawY: Float = 0f
    private var dX = 0f
    private var dY: Float = 0f

    private var cheatBox: CheatBox? = null

    init {
        val orientation = activity.resources.configuration.orientation
        if (orientation == android.content.res.Configuration.ORIENTATION_LANDSCAPE) {
            x = parentView.width / 2f
            y = 0f
        } else {
            x = 0f
            y = parentView.height / 2f
        }
        setOnTouchListener(this)
        setImageResource(com.ee.command_receiver.R.drawable.ic_cheat)
        setOnClickListener {
            if (cheatBox == null) {
                createCheatBox()
                setImageResource(com.ee.command_receiver.R.drawable.ic_close)
            } else {
                removeCheatBox()
                setImageResource(com.ee.command_receiver.R.drawable.ic_cheat)
            }
        }

        layoutParams =
            ViewGroup.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT).apply {
                scaleType = ScaleType.CENTER
            }

        val viewGroup = activity.window.decorView as ViewGroup
        viewGroup.addView(this)
    }

    override fun onTouch(view: View, motionEvent: MotionEvent): Boolean {
        return when (motionEvent.action) {
            MotionEvent.ACTION_DOWN -> {
                return onTouchStart(view, motionEvent)
            }

            MotionEvent.ACTION_MOVE -> {
                return onTouchMove(view, motionEvent)
            }

            MotionEvent.ACTION_UP -> {
                return onTouchEnd(view, motionEvent)
            }

            else -> {
                super.onTouchEvent(motionEvent)
            }
        }
    }

    private fun onTouchStart(
        view: View,
        motionEvent: MotionEvent
    ): Boolean {
        downRawX = motionEvent.rawX
        downRawY = motionEvent.rawY
        dX = view.x - downRawX
        dY = view.y - downRawY
        return true // Consumed
    }

    private fun onTouchEnd(
        view: View,
        motionEvent: MotionEvent
    ): Boolean {
        val upRawX = motionEvent.rawX
        val upRawY = motionEvent.rawY
        val upDX = upRawX - downRawX
        val upDY: Float = upRawY - downRawY
        if (abs(upDX) < CLICK_DRAG_TOLERANCE && abs(upDY) < CLICK_DRAG_TOLERANCE) { // A click
            log("onTouchEnd: click")
            return performClick()
        } else { // A drag
            return true // Consumed
        }
    }

    private fun onTouchMove(
        view: View,
        motionEvent: MotionEvent
    ): Boolean {
        val viewWidth = view.width
        val viewHeight = view.height
        val viewParent = view.parent as View
        val parentWidth = viewParent.width
        val parentHeight = viewParent.height

        var newX = motionEvent.rawX + dX
        var newY = motionEvent.rawY + dY
        newX = clamp(newX, 0f, (parentWidth - viewWidth).toFloat())
        newY = clamp(newY, 0f, (parentHeight - viewHeight).toFloat())

        view.x = newX
        view.y = newY

        return true // Consumed
    }

    private fun createCheatBox() {
        if (cheatBox != null) {
            return
        }
        val box = CheatBox(activity)
        cheatBox = box

        box.addCloseButton(::removeCheatBox)
        box.addButton("Test")
        box.addButton("Test2")
        box.addButton("How to show the close button on top right corner?")
        box.addButton("How to show the close button on top right corner?")
        box.addButton("Test3")
        box.addButton("How to show the close button on top right corner?")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("How to show the close button on top right corner?")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
        box.addButton("Test3")
    }

    private fun removeCheatBox() {
        if (cheatBox == null) {
            return
        }
        cheatBox?.removeSelf()
        cheatBox = null
    }

    private fun clamp(value: Float, min: Float, max: Float): Float {
        return value.coerceAtLeast(min).coerceAtMost(max)
    }

    private fun log(message: String) {
        Log.d("nhanc19", message)
    }
}