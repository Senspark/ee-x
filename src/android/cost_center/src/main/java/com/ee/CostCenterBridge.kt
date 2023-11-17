package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread

class CostCenterBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = CostCenterBridge::class.java.name
        private const val kPrefix = "CostCenterBridge"
        private const val kInitialize = "${kPrefix}Initialize"
    }

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) {
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }
}