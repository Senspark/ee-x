package com.ee

import android.app.Activity
import android.content.Context
import com.google.firebase.FirebaseApp

/**
 * Created by Zinge on 7/4/16.
 */
class FirebaseCoreBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebaseCoreBridge::class.java.name
    }

    init {
        _logger.info("$kTag: constructor begin.")
        FirebaseApp.initializeApp(_context)
        _logger.info("$kTag: constructor end.")
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}
    override fun destroy() {}
}