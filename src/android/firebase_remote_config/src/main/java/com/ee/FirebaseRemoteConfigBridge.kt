package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.google.firebase.ktx.Firebase
import com.google.firebase.remoteconfig.FirebaseRemoteConfig
import com.google.firebase.remoteconfig.ktx.remoteConfig
import com.google.firebase.remoteconfig.ktx.remoteConfigSettings
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.JsonPrimitive
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
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetSettings = "${kPrefix}SetSettings"
        private const val kFetch = "${kPrefix}Fetch"
        private const val kActivate = "${kPrefix}Activate"
        private const val kSetDefaults = "${kPrefix}SetDefaults"
        private const val kGetBool = "${kPrefix}GetBool"
        private const val kGetLong = "${kPrefix}GetLong"
        private const val kGetDouble = "${kPrefix}GetDouble"
        private const val kGetString = "${kPrefix}GetString"
    }

    private var _plugin: FirebaseRemoteConfig? = null

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
        _bridge.registerAsyncHandler(kInitialize) {
            Utils.toString(initialize())
            ""
        }
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
            fetch(request.fetchInterval).toString()
        }
        _bridge.registerAsyncHandler(kActivate) {
            Utils.toString(activate())
        }
        _bridge.registerAsyncHandler(kSetDefaults) { message ->
            @Serializable
            class Request(
                val defaults: Map<String, JsonPrimitive>,
            )

            val request = deserialize<Request>(message)
            setDefaults(request.defaults)
            ""
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
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetSettings)
        _bridge.deregisterHandler(kFetch)
        _bridge.deregisterHandler(kActivate)
        _bridge.deregisterHandler(kSetDefaults)
        _bridge.deregisterHandler(kGetBool)
        _bridge.deregisterHandler(kGetLong)
        _bridge.deregisterHandler(kGetDouble)
        _bridge.deregisterHandler(kGetString)
    }

    @AnyThread
    private suspend fun initialize(): Boolean {
        return FirebaseInitializer.instance.initialize(true) && suspendCoroutine { cont ->
            Thread.runOnMainThread {
                Firebase.remoteConfig.ensureInitialized().addOnCompleteListener {
                    _plugin = Firebase.remoteConfig
                    cont.resume(true)
                }
            }
        }
    }

    @AnyThread
    private suspend fun setSettings(fetchTimeOut: Long, fetchInterval: Long) {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                plugin.setConfigSettingsAsync(remoteConfigSettings {
                    fetchTimeoutInSeconds = fetchTimeOut
                    minimumFetchIntervalInSeconds = fetchInterval
                })
                cont.resume(Unit)
            }
        }
    }

    @AnyThread
    private suspend fun fetch(fetchInterval: Long): Int {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                plugin.fetch(fetchInterval).addOnCompleteListener {
                    Thread.runOnMainThread {
                        when (Firebase.remoteConfig.info.lastFetchStatus) {
                            FirebaseRemoteConfig.LAST_FETCH_STATUS_SUCCESS -> cont.resume(0)
                            FirebaseRemoteConfig.LAST_FETCH_STATUS_NO_FETCH_YET -> cont.resume(1)
                            FirebaseRemoteConfig.LAST_FETCH_STATUS_FAILURE -> cont.resume(2)
                            FirebaseRemoteConfig.LAST_FETCH_STATUS_THROTTLED -> cont.resume(3)
                        }
                    }
                }
            }
        }
    }

    @AnyThread
    private suspend fun activate(): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                plugin.activate().addOnCompleteListener { result ->
                    Thread.runOnMainThread {
                        cont.resume(result.isSuccessful && result.result)
                    }
                }
            }
        }
    }

    @AnyThread
    private suspend fun setDefaults(defaults: Map<String, JsonPrimitive>) {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                plugin.setDefaultsAsync(defaults).addOnCompleteListener {
                    Thread.runOnMainThread {
                        cont.resume(Unit)
                    }
                }
            }
        }
    }

    @AnyThread
    private suspend fun getBool(key: String): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                cont.resume(plugin.getBoolean(key))
            }
        }
    }

    @AnyThread
    private suspend fun getLong(key: String): Long {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                cont.resume(plugin.getLong(key))
            }
        }
    }

    @AnyThread
    private suspend fun getDouble(key: String): Double {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                cont.resume(plugin.getDouble(key))
            }
        }
    }

    @AnyThread
    private suspend fun getString(key: String): String {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                val plugin = _plugin
                    ?: throw IllegalStateException("Please call initialize() first")
                cont.resume(plugin.getString(key))
            }
        }
    }
}