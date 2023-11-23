package com.ee.cheat

import android.app.Activity
import android.util.Log
import android.view.ViewGroup
import android.widget.Button
import android.widget.LinearLayout
import android.widget.RelativeLayout
import androidx.core.view.children
import com.ee.command_receiver.databinding.CheatBoxBinding

class CheatBox(
    activity: Activity
) : RelativeLayout(activity) {
    private val binding: CheatBoxBinding

    init {
        val viewGroup = activity.window.decorView as ViewGroup
        binding = CheatBoxBinding.inflate(activity.layoutInflater, this, true)
        viewGroup.addView(this)
    }

    fun removeSelf() {
        (parent as ViewGroup).removeView(this)
    }

    fun addCloseButton(onCloseClicked: () -> Unit) {
        binding.btnClose.setOnClickListener {
            onCloseClicked()
        }
    }

    fun addButton(text: String) {
        val button = Button(context).apply {
            layoutParams = LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
            )
            this.text = text
        }
        binding.container.addView(button)
    }
}