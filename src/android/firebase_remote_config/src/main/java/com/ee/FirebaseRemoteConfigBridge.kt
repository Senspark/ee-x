package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.google.firebase.ktx.Firebase
import com.google.firebase.remoteconfig.ktx.remoteConfig
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

class FirebaseRemoteConfigBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebaseRemoteConfigBridge::class.java.name
        private const val kPrefix = "FirebaseRemoteConfigBridge"
    }

    private val _config = Firebase.remoteConfig

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
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
    }

    @AnyThread
    private fun deregisterHandlers() {
    }

    @AnyThread
    private suspend fun fetch() {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _config.fetch().addOnSuccessListener {
                    cont.resume(Unit)
                }
            }
        }
    }

    @AnyThread
    private suspend fun getValue(key: String): String {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                cont.resume(_config.getString(key))
            }
        }
    }
}