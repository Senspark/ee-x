package com.ee.ui

import android.app.Activity
import android.view.ViewGroup
import android.widget.LinearLayout
import android.widget.RelativeLayout
import androidx.appcompat.widget.AppCompatButton
import androidx.appcompat.widget.AppCompatEditText
import com.ee.ILogger
import com.ee.command_receiver.databinding.ListBoxBinding
import org.json.JSONObject

class ListBox(
    activity: Activity,
    private val _logger: ILogger,
    private val _callback: (String) -> Unit
) : RelativeLayout(activity) {
    private val _binding: ListBoxBinding
    private var _commandsId = 0;
    private val _commands = mutableMapOf<Int, CmdData>()
    private val _editTexts = mutableMapOf<Int, android.widget.EditText>()

    companion object {
        private const val kTag = "CheatBox"
    }

    init {
        val viewGroup = activity.window.decorView as ViewGroup
        _binding = ListBoxBinding.inflate(activity.layoutInflater, this, true)
        viewGroup.addView(this)
    }

    fun removeSelf() {
        (parent as ViewGroup).removeView(this)
    }

    fun addCloseButton(onCloseClicked: () -> Unit) {
        _binding.btnClose.setOnClickListener {
            onCloseClicked()
        }
    }

    fun addRow(jsonData: String) {
        try {
            val json = JSONObject(jsonData)
            val cmd = json.getString("cmd")
            val sample = json.getString("sample")
            val desc = json.getString("desc")
            val id = _commandsId++

            _commands[id] = CmdData(desc, cmd, sample)

            // Add Linear Layout
            val row = LinearLayout(context).apply {
                layoutParams = LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.MATCH_PARENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
                )
                orientation = LinearLayout.HORIZONTAL
            }

            // Add Button
            val button = AppCompatButton(context).apply {
                layoutParams = LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.WRAP_CONTENT,
                    LinearLayout.LayoutParams.WRAP_CONTENT
                )
                this.text = desc
                this.tag = id
                setOnClickListener {
                    onBtnClicked(it.tag as Int)
                }
            }
            row.addView(button)

            // Add EditText
            if (!sample.isNullOrBlank() && sample != "_") {
                val editText = AppCompatEditText(context).apply {
                    layoutParams = LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.WRAP_CONTENT,
                        LinearLayout.LayoutParams.WRAP_CONTENT
                    )
                    if (sample.toIntOrNull() != null) {
                        this.inputType = android.text.InputType.TYPE_CLASS_NUMBER
                    } else {
                        this.inputType = android.text.InputType.TYPE_CLASS_TEXT
                    }
                    this.setText(sample)
                }
                _editTexts[id] = editText
                row.addView(editText)
            }

            _binding.container.addView(row)
        } catch (e: Exception) {
            _logger.error("$kTag: addButton: $e")
        }
    }

    private fun onBtnClicked(id: Int) {
        val cmdData = _commands[id]
        if (cmdData != null) {

            val json = JSONObject()
            json.put("cmd", cmdData.cmd)

            var data = cmdData.sample
            if (_editTexts.containsKey(id)) {
                val value = _editTexts[id]?.text.toString()
                if (value.isNotBlank()) {
                    data = value
                }
            }
            json.put("data", data)

            val output = json.toString()
            _logger.info("$kTag: onBtnClicked: $output")
            _callback(output)
        }
    }
}

data class CmdData(
    val desc: String,
    val cmd: String,
    val sample: String
)