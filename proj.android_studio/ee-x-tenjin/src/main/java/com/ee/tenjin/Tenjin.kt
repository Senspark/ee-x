package com.ee.tenjin

import android.app.Activity
import android.content.Context
import android.content.Intent
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.registerHandler
import com.tenjin.android.TenjinSDK

class Tenjin(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(Tenjin::class.java.name)

        private const val k__initialize = "Tenjin_initialize"
    }

    private var _tenjin: TenjinSDK? = null

    init {
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {
        _tenjin?.connect()
    }

    override fun onPause() {}
    override fun onDestroy() {
        _activity = null
    }

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        return false
    }

    override fun destroy() {
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            _tenjin = null
        })
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__initialize) { message ->
            initialize(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize)
    }

    @UiThread
    private fun initialize(apiKey: String) {
        Thread.checkMainThread()
        _tenjin = TenjinSDK.getInstance(_activity, apiKey)
        _tenjin?.connect()
    }
}