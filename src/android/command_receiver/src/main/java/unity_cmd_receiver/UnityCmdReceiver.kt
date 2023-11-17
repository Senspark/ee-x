package com.senspark.unity_cmd_receiver

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.util.Log
import org.json.JSONObject

class UnityCmdReceiver : BroadcastReceiver() {
    private val kActionName = "com.senspark.ACTION_CMD"
    private val kCmd = "cmd"
    private val kData = "data"
    private val kTag = UnityCmdReceiver::class.java.name

    companion object {
        var callback: ((message: String) -> Unit)? = null
    }

    override fun onReceive(context: Context?, intent: Intent?) {
        if (intent != null) {
            if (!intent.action.equals(kActionName)) {
                return
            }
            val cmd = intent.getStringExtra(kCmd)
            val data = intent.getStringExtra(kData)
            if (cmd.isNullOrEmpty()) {
                return
            }
            Log.d(kTag, "Received Cmd $cmd $data")
            broadcastMessage(cmd, data)
        }
    }

    private fun broadcastMessage(cmd: String, data: String?) {
        val jsonObject = JSONObject()
        jsonObject.put(kCmd, cmd)
        jsonObject.put(kData, data)
        callback?.invoke(jsonObject.toString())
    }
}