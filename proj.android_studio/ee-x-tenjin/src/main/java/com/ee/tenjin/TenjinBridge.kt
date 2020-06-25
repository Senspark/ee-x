package com.ee.tenjin

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.registerHandler
import com.tenjin.android.TenjinSDK

class TenjinBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(TenjinBridge::class.java.name)

        private const val kPrefix = "TenjinBridge"
        private const val kInitialize = "${kPrefix}Initialize"
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

    override fun destroy() {
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            _tenjin = null
        })
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            initialize(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }

    @AnyThread
    private fun initialize(apiKey: String) {
        Thread.runOnMainThread(Runnable {
            _tenjin = TenjinSDK.getInstance(_activity, apiKey)
            _tenjin?.connect()
        })
    }
}