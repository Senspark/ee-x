package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.google.firebase.crashlytics.FirebaseCrashlytics

class FirebaseCrashlyticsBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebaseCrashlyticsBridge::class.java.name
        private const val kPrefix = "FirebaseCrashlyticsBridge"
        private const val kLog = "${kPrefix}Log"
    }

    private val _crashlytics = FirebaseCrashlytics.getInstance()

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end.")
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