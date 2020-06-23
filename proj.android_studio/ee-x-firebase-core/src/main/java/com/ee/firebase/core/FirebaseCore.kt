package com.ee.firebase.core

import android.app.Activity
import android.content.Context
import android.content.Intent
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.google.firebase.FirebaseApp

/**
 * Created by Zinge on 7/4/16.
 */
class FirebaseCore(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(FirebaseCore::class.java.name)
    }

    init {
        _logger.debug("constructor begin.")
        FirebaseApp.initializeApp(_context)
        _logger.debug("constructor end.")
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}
    override fun destroy() {}
}