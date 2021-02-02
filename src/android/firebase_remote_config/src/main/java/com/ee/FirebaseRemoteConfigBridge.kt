package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.google.firebase.ktx.Firebase
import com.google.firebase.remoteconfig.ktx.remoteConfig
import com.google.firebase.remoteconfig.ktx.remoteConfigSettings
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

@InternalSerializationApi
class FirebaseRemoteConfigBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FirebaseRemoteConfigBridge::class.java.name
        private const val kPrefix = "FirebaseRemoteConfigBridge"
        private const val kSetSettings = "${kPrefix}SetSettings"
        private const val kFetch = "${kPrefix}Fetch"
        private const val kActivate = "${kPrefix}Activate"
        private const val kGetBool = "${kPrefix}GetBool"
        private const val kGetLong = "${kPrefix}GetLong"
        private const val kGetDouble = "${kPrefix}GetDouble"
        private const val kGetString = "${kPrefix}GetString"
        private const val kSetDefaults = "${kPrefix}SetDefaults"
    }

    private val _config = Firebase.remoteConfig

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

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kSetSettings) { message ->
            @Serializable
            class Request(
                val fetchTimeOut: Long,
                val fetchInterval: Long,
            )

            val request = deserialize<Request>(message)
            setSettings(request.fetchTimeOut, request.fetchInterval)
            ""
        }
        _bridge.registerAsyncHandler(kFetch) { message ->
            @Serializable
            class Request(
                val fetchInterval: Long,
            )

            val request = deserialize<Request>(message)
            fetch(request.fetchInterval)
            ""
        }
        _bridge.registerAsyncHandler(kActivate) {
            Utils.toString(activate())
        }
        _bridge.registerAsyncHandler(kGetBool) { message ->
            Utils.toString(getBool(message))
        }
        _bridge.registerAsyncHandler(kGetLong) { message ->
            getLong(message).toString()
        }
        _bridge.registerAsyncHandler(kGetDouble) { message ->
            getDouble(message).toString()
        }
        _bridge.registerAsyncHandler(kGetString) { message ->
            getString(message)
        }
        _bridge.registerAsyncHandler(kSetDefaults) { message ->
            @Serializable
            class Request(
                val defaults: Map<String, String>,
            )

            val request = deserialize<Request>(message)
            setDefaults(request.defaults)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kSetSettings)
        _bridge.deregisterHandler(kFetch)
        _bridge.deregisterHandler(kActivate)
        _bridge.deregisterHandler(kGetBool)
        _bridge.deregisterHandler(kGetLong)
        _bridge.deregisterHandler(kGetDouble)
        _bridge.deregisterHandler(kGetString)
        _bridge.deregisterHandler(kSetDefaults)
    }

    @AnyThread
    private suspend fun setSettings(fetchTimeOut: Long, fetchInterval: Long) {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _config.setConfigSettingsAsync(remoteConfigSettings {
                    fetchTimeoutInSeconds = fetchTimeOut
                    minimumFetchIntervalInSeconds = fetchInterval
                })
                cont.resume(Unit)
            }
        }
    }

    @AnyThread
    private suspend fun fetch(fetchInterval: Long) {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _config.fetch(fetchInterval).addOnSuccessListener {
                    Thread.runOnMainThread {
                        cont.resume(Unit)
                    }
                }
            }
        }
    }

    @AnyThread
    private suspend fun activate(): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _config.activate().addOnSuccessListener { result ->
                    Thread.runOnMainThread {
                        cont.resume(result)
                    }
                }
            }
        }
    }

    @AnyThread
    private suspend fun getBool(key: String): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                cont.resume(_config.getBoolean(key))
            }
        }
    }

    @AnyThread
    private suspend fun getLong(key: String): Long {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                cont.resume(_config.getLong(key))
            }
        }
    }

    @AnyThread
    private suspend fun getDouble(key: String): Double {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                cont.resume(_config.getDouble(key))
            }
        }
    }

    @AnyThread
    private suspend fun getString(key: String): String {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                cont.resume(_config.getString(key))
            }
        }
    }

    @AnyThread
    private suspend fun setDefaults(defaults: Map<String, String>) {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _config.setDefaultsAsync(defaults)
                cont.resume(Unit)
            }
        }
    }
}