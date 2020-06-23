package com.ee.firebase.crashlytics

import android.app.Activity
import android.content.Context
import android.content.Intent
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.registerHandler

class FirebaseCrashlytics(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(FirebaseCrashlytics::class.java.name)

        private const val kLog = "FirebaseCrashlytics_log"
    }

    private val _crashlytics = com.google.firebase.crashlytics.FirebaseCrashlytics.getInstance()

    init {
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kLog) { message ->
            log(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kLog)
    }

    @AnyThread
    fun log(message: String) {
        _crashlytics.log(message)
    }
}