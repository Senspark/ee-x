package com.ee

import android.app.Activity
import android.app.Application
import android.view.ViewGroup
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
    }

    private lateinit var fab: com.ee.cheat.Fab

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        UnityCmdReceiver.callback = { message ->
            onMessageReceived(message)
        }
        if(_activity != null) {
            val rootView = _activity!!.window.decorView.rootView
            val listener = ViewTreeObserver.OnGlobalLayoutListener {
                if (!::fab.isInitialized) {
                    val activity = _activity!!
                    val density = activity.resources.displayMetrics.density
                    val sizeInPx = (48 * density).toInt()
                    fab = com.ee.cheat.Fab(activity, rootView)
                    fab.layoutParams = ViewGroup.LayoutParams(sizeInPx, sizeInPx)
                    val viewGroup = activity.window.decorView as ViewGroup
                    viewGroup.addView(fab)
                }
            }
            rootView.viewTreeObserver.addOnGlobalLayoutListener(listener)
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