package com.ee

import android.app.Activity
import android.app.Application
import android.view.ViewTreeObserver
import androidx.annotation.Keep

@Keep
class MReceiverBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = MReceiverBridge::class.java.name
        private const val kPrefix = "MReceiverBridge"
        private const val kOnMessageReceived = "${kPrefix}OnMessageReceived"
        private const val kAddCommand = "${kPrefix}AddCommand"
        private const val kShowUI = "${kPrefix}ShowUI"
    }

    private lateinit var fab: com.ee.ui.Fab
    private val _cachedMessages = mutableListOf<String>()

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

    private fun onMessageReceived(message: String) {
        _logger.info("$kTag: onMessageReceived: message = $message")
        _bridge.callCpp(kOnMessageReceived, message)
    }

    private fun registerHandlers() {
        _bridge.registerHandler(kAddCommand) { message ->
            if (::fab.isInitialized) {
                fab.addCommand(message)
            } else {
                _cachedMessages.add(message)
            }
            ""
        }
        _bridge.registerHandler(kShowUI) {
            if (_activity != null) {
                initFab(_activity!!)
            }
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kAddCommand)
        _bridge.deregisterHandler(kShowUI)
    }

    private fun initFab(activity: Activity) {
        val rootView = activity.window.decorView.rootView
        val listener = ViewTreeObserver.OnGlobalLayoutListener {
            if (!::fab.isInitialized) {
                fab = com.ee.ui.Fab(activity, rootView, _logger, ::onMessageReceived)
                for (message in _cachedMessages) {
                    fab.addCommand(message)
                }
                _cachedMessages.clear()
            }
        }
        rootView.viewTreeObserver.addOnGlobalLayoutListener(listener)
    }
}