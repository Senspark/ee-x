package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.google.common.truth.Truth.assertThat
import com.unity3d.ads.IUnityAdsInitializationListener
import com.unity3d.ads.IUnityAdsListener
import com.unity3d.ads.IUnityAdsLoadListener
import com.unity3d.ads.UnityAds
import com.unity3d.ads.UnityAds.FinishState
import com.unity3d.ads.UnityAds.UnityAdsError
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.util.Collections
import java.util.concurrent.ConcurrentHashMap
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

@InternalSerializationApi
class UnityAdsBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin, IUnityAdsListener {
    companion object {
        private val kTag = UnityAdsBridge::class.java.name
        private const val kPrefix = "UnityAdsBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetDebugModeEnabled = "${kPrefix}SetDebugModeEnabled"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kLoadRewardedAd = "${kPrefix}LoadRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
        private const val kOnLoaded = "${kPrefix}OnLoaded"
        private const val kOnFailedToShow = "${kPrefix}OnFailedToShow"
        private const val kOnClosed = "${kPrefix}OnClosed"
    }

    private var _initializing = false
    private var _initialized = false
    private val _loadedAdIds: MutableSet<String> = Collections.newSetFromMap(ConcurrentHashMap())

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
        Thread.runOnMainThread {
            UnityAds.removeListener(this)
        }
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            @Serializable
            class Request(
                val gameId: String,
                val testModeEnabled: Boolean
            )

            val request = deserialize<Request>(message)
            Utils.toString(initialize(request.gameId, request.testModeEnabled))
        }
        _bridge.registerHandler(kSetDebugModeEnabled) { message ->
            setDebugModeEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kHasRewardedAd) { message ->
            Utils.toString(hasRewardedAd(message))
        }
        _bridge.registerAsyncHandler(kLoadRewardedAd) { message ->
            Utils.toString(loadRewardedAd(message))
        }
        _bridge.registerHandler(kShowRewardedAd) { message ->
            showRewardedAd(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetDebugModeEnabled)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    @AnyThread
    suspend fun initialize(gameId: String, testModeEnabled: Boolean): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                if (!UnityAds.isSupported()) {
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (UnityAds.isInitialized()) {
                    _initialized = true
                    cont.resume(true)
                    return@runOnMainThread
                }
                if (_initializing) {
                    cont.resume(false)
                    return@runOnMainThread
                }
                _initializing = true
                UnityAds.initialize(_application, gameId, testModeEnabled, true, object : IUnityAdsInitializationListener {
                    override fun onInitializationComplete() {
                        Thread.runOnMainThread {
                            _logger.info(this::onInitializationComplete.name)
                            _initializing = false
                            _initialized = true
                            cont.resume(true)
                        }
                    }

                    override fun onInitializationFailed(error: UnityAds.UnityAdsInitializationError?, message: String?) {
                        Thread.runOnMainThread {
                            _logger.error("${this::onInitializationFailed.name}: error = ${error ?: ""} message = ${message ?: ""}")
                            _initializing = false
                            cont.resume(false)
                        }
                    }
                })
            }
        }
    }

    @AnyThread
    private fun setDebugModeEnabled(enabled: Boolean) {
        Thread.runOnMainThread {
            if (!_initialized) {
                throw IllegalStateException("Please call initialize() first")
            }
            UnityAds.setDebugMode(enabled)
        }
    }

    @AnyThread
    fun hasRewardedAd(adId: String): Boolean {
        if (!_initialized) {
            // Changed on main thread.
            return false
        }
        return _loadedAdIds.contains(adId)
    }

    @AnyThread
    suspend fun loadRewardedAd(adId: String): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _logger.debug("$kTag: loadRewardedAd: $adId")
                if (!_initialized) {
                    throw IllegalStateException("Please call initialize() first")
                }
                UnityAds.load(adId, object : IUnityAdsLoadListener {
                    override fun onUnityAdsAdLoaded(placementId: String?) {
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: ${this::onUnityAdsAdLoaded.name}: $adId")
                            cont.resume(true)
                        }
                    }

                    override fun onUnityAdsFailedToLoad(placementId: String?) {
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: ${this::onUnityAdsFailedToLoad.name}: $adId")
                            cont.resume(false)
                        }
                    }
                })
            }
        }
    }

    @AnyThread
    fun showRewardedAd(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::showRewardedAd.name}")
            if (!_initialized) {
                throw IllegalStateException("Please call initialize() first")
            }
            UnityAds.addListener(this)
            UnityAds.show(_activity, adId)
        }
    }

    override fun onUnityAdsReady(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsReady.name}: $adId")
            _loadedAdIds.add(adId)
            _bridge.callCpp(kOnLoaded, adId)
        }
    }

    override fun onUnityAdsStart(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsStart.name}: $adId")
            _loadedAdIds.remove(adId)
        }
    }

    override fun onUnityAdsFinish(adId: String, state: FinishState) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsFinish.name}: $adId state = $state")
            UnityAds.removeListener(this)
            if (state == FinishState.ERROR) {
                @Serializable
                @Suppress("unused")
                class ResponseA(
                    val ad_id: String,
                    val message: String
                )

                val response = ResponseA(adId, "")
                _bridge.callCpp(kOnFailedToShow, response.serialize())
                return@runOnMainThread
            }
            if (state == FinishState.SKIPPED) {
                @Serializable
                @Suppress("unused")
                class ResponseB(
                    val ad_id: String,
                    val rewarded: Boolean
                )

                val response = ResponseB(adId, false)
                _bridge.callCpp(kOnClosed, response.serialize())
                return@runOnMainThread
            }
            if (state == FinishState.COMPLETED) {
                @Serializable
                @Suppress("unused")
                class ResponseC(
                    val ad_id: String,
                    val rewarded: Boolean
                )

                val response = ResponseC(adId, true)
                _bridge.callCpp(kOnClosed, response.serialize())
                return@runOnMainThread
            }
            assertThat(false).isTrue()
        }
    }

    override fun onUnityAdsError(unityAdsError: UnityAdsError, message: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsError.name}: error = $unityAdsError message = $message")
            UnityAds.removeListener(this)
        }
    }
}