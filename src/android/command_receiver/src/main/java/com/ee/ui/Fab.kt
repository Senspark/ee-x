package com.ee.ui

import android.app.ActionBar.LayoutParams
import android.app.Activity
import android.view.MotionEvent
import android.view.View
import android.view.ViewGroup
import com.ee.ILogger
import com.google.android.material.floatingactionbutton.FloatingActionButton
import kotlin.math.abs

class Fab(
    private val _activity: Activity,
    private val _parentView: View,
    private val _logger: ILogger,
    private val _onCmdCallback: (String) -> Unit
) :
    FloatingActionButton(_activity),
    View.OnTouchListener {

    companion object {
        private const val CLICK_DRAG_TOLERANCE =
            10f // Often, there will be a slight, unintentional, drag when the user taps the FAB, so we need to account for this.
    }

    private var downRawX = 0f
    private var downRawY: Float = 0f
    private var dX = 0f
    private var dY: Float = 0f

    private var _listBox: ListBox? = null
    private val _cachedCommands = mutableListOf<String>()

    init {
        setOnTouchListener(this)
        setImageResource(com.ee.command_receiver.R.drawable.ic_config)
        setOnClickListener {
            if (_listBox == null) {
                createCheatBox()
                setImageResource(com.ee.command_receiver.R.drawable.ic_close)
            } else {
                removeCheatBox()
                setImageResource(com.ee.command_receiver.R.drawable.ic_config)
            }
        }

        layoutParams =
            ViewGroup.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT).apply {
                scaleType = ScaleType.CENTER
            }

        val viewGroup = _activity.window.decorView as ViewGroup
        viewGroup.addView(this)
    }

    fun addCommand(jsonData: String) {
        _cachedCommands.add(jsonData)
    }

    override fun onSizeChanged(w: Int, h: Int, oldw: Int, oldh: Int) {
        super.onSizeChanged(w, h, oldw, oldh)
        val orientation = _activity.resources.configuration.orientation
        if (orientation == android.content.res.Configuration.ORIENTATION_LANDSCAPE) {
            x = (_parentView.width - w).toFloat()
            y = (_parentView.height / 2f) - h / 2f
        } else {
            x = (_parentView.width / 2f) - w / 2f
            y = (_parentView.height - h).toFloat()
        }
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
        if (_listBox != null) {
            return
        }
        val box = ListBox(_activity, _logger, _onCmdCallback)
        for (command in _cachedCommands) {
            box.addRow(command)
        }
        _listBox = box

        box.addCloseButton(::removeCheatBox)
    }

    private fun removeCheatBox() {
        if (_listBox == null) {
            return
        }
        _listBox?.removeSelf()
        _listBox = null
    }

    private fun clamp(value: Float, min: Float, max: Float): Float {
        return value.coerceAtLeast(min).coerceAtMost(max)
    }
}