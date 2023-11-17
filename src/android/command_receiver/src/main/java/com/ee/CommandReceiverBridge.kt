package com.ee

import android.app.Activity
import android.app.Application
import com.senspark.unity_cmd_receiver.UnityCmdReceiver

class CommandReceiverBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = CommandReceiverBridge::class.java.name
        private const val kPrefix = "CommandReceiverBridge"
        private const val kOnMessageReceived = "${kPrefix}OnMessageReceived"
    }

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        UnityCmdReceiver.callback = { message ->
            onMessageReceived(message)
        }
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

    private fun onMessageReceived(message: String) {
        _logger.info("$kTag: onMessageReceived: message = $message")
        _bridge.callCpp(kOnMessageReceived, message)
    }

    private fun registerHandlers() {
    }

    private fun deregisterHandlers() {
    }
}