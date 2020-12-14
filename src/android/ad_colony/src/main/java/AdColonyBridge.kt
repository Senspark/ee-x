package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.adcolony.sdk.AdColony
import com.ee.internal.deserialize
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

@InternalSerializationApi
class AdColonyBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = AdColonyBridge::class.java.name
        private const val kPrefix = "AdColonyBridge"
        private const val kInitialize = "${kPrefix}Initialize"
    }

    private var _initializing = false
    private var _initialized = false

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end.")
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
        _bridge.registerAsyncHandler(kInitialize) { message ->
            @Serializable
            class Request(
                val appId: String,
                val zoneIds: List<String>
            )

            val request = deserialize<Request>(message)
            Utils.toString(initialize(request.appId, request.zoneIds))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }

    @AnyThread
    suspend fun initialize(appId: String, zoneIds: List<String>): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                if (_initializing) {
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (_initialized) {
                    cont.resume(true)
                    return@runOnMainThread
                }
                _initializing = true
                val result = AdColony.configure(_activity, appId, *zoneIds.toTypedArray())
                _initializing = false
                if (result) {
                    _initialized = true
                }
                _logger.info("$kTag: initialize: result = $result version = ${AdColony.getSDKVersion()}")
                cont.resume(true)
            }
        }
    }
}