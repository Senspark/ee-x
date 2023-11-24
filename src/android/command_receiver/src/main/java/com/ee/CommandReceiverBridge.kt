package com.ee

import android.app.Activity
import android.app.Application
import android.view.ViewTreeObserver
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
        private const val kAddCommand = "${kPrefix}AddCommand"
    }

    private lateinit var fab: com.ee.cheat.Fab
    private val _cachedMessages = mutableListOf<String>()

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        UnityCmdReceiver.callback = { message ->
            onMessageReceived(message)
        }
        // Init Fab
        if (_activity != null) {
            val rootView = _activity!!.window.decorView.rootView
            val listener = ViewTreeObserver.OnGlobalLayoutListener {
                if (!::fab.isInitialized) {
                    fab = com.ee.cheat.Fab(_activity!!, rootView, _logger, ::onMessageReceived)
                    for (message in _cachedMessages) {
                        fab.addCommand(message)
                    }
                    _cachedMessages.clear()
                }
            }
            rootView.viewTreeObserver.addOnGlobalLayoutListener(listener)
        }
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

    private fun onMessageReceived(message: String) {
        _logger.info("$kTag: onMessageReceived: message = $message")
        _bridge.callCpp(kOnMessageReceived, message)
    }

    private fun registerHandlers() {
        _bridge.registerHandler(kAddCommand) { message ->
            _logger.info("$kTag: addCommand: message = $message")
            if (::fab.isInitialized) {
                fab.addCommand(message)
            } else {
                _logger.info("$kTag: addCommand: cache message")
                _cachedMessages.add(message)
            }
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kAddCommand)
    }
}