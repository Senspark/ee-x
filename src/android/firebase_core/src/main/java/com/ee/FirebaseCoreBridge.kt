package com.ee

import android.app.Activity
import android.app.Application
import com.google.firebase.FirebaseApp

/**
 * Created by Zinge on 7/4/16.
 */
class FirebaseCoreBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebaseCoreBridge::class.java.name
    }

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        FirebaseApp.initializeApp(_application)
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        _activity = null
    }
}