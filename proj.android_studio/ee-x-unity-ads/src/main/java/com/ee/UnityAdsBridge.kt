package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.google.common.truth.Truth.assertThat
import com.unity3d.ads.IUnityAdsListener
import com.unity3d.ads.UnityAds
import com.unity3d.ads.UnityAds.FinishState
import com.unity3d.ads.UnityAds.UnityAdsError
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.util.*
import java.util.concurrent.ConcurrentHashMap

@InternalSerializationApi
class UnityAdsBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(UnityAdsBridge::class.java.name)

        private const val kPrefix = "UnityAdsBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetDebugModeEnabled = "${kPrefix}SetDebugModeEnabled"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
        private const val kOnLoaded = "${kPrefix}OnLoaded"
        private const val kOnFailedToShow = "${kPrefix}OnFailedToShow"
        private const val kOnClosed = "${kPrefix}OnClosed"
    }

    private var _initialized = false
    private var _listener: IUnityAdsListener? = null
    private val _loadedAdIds: MutableSet<String> = Collections.newSetFromMap(ConcurrentHashMap())

    init {
        _logger.debug("constructor begin.")
        registerHandlers()
        _logger.debug("constructor end.")
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
            if (!_initialized) {
                return@runOnMainThread
            }
            UnityAds.removeListener(_listener)
        }
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            @Serializable
            class Request(
                val gameId: String,
                val testModeEnabled: Boolean
            )

            val request = deserialize<Request>(message)
            initialize(request.gameId, request.testModeEnabled)
            ""
        }
        _bridge.registerHandler(kSetDebugModeEnabled) { message ->
            setDebugModeEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kHasRewardedAd) { message ->
            Utils.toString(hasRewardedAd(message))
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
    fun initialize(gameId: String, testModeEnabled: Boolean) {
        Thread.runOnMainThread {
            if (_initialized) {
                return@runOnMainThread
            }
            if (!UnityAds.isSupported()) {
                return@runOnMainThread
            }
            _listener = object : IUnityAdsListener {
                override fun onUnityAdsReady(adId: String) {
                    _logger.info("${this::onUnityAdsReady.name}: $adId")
                    Thread.checkMainThread()
                    _loadedAdIds.add(adId)
                    _bridge.callCpp(kOnLoaded, adId)
                }

                override fun onUnityAdsStart(adId: String) {
                    _logger.info("${this::onUnityAdsStart.name}: $adId")
                    Thread.checkMainThread()
                }

                override fun onUnityAdsFinish(adId: String, state: FinishState) {
                    _logger.info("${this::onUnityAdsFinish.name}: $adId state = $state")
                    Thread.checkMainThread()
                    if (state == FinishState.ERROR) {
                        @Serializable
                        @Suppress("unused")
                        class ResponseA(
                            val ad_id: String,
                            val message: String
                        )

                        val response = ResponseA(adId, "")
                        _bridge.callCpp(kOnFailedToShow, response.serialize())
                        return
                    }
                    if (state == FinishState.SKIPPED) {
                        @Serializable
                        @Suppress("unused")
                        class ResponseB(
                            val ad_id: String,
                            val rewarded: Boolean
                        )

                        _loadedAdIds.remove(adId)
                        val response = ResponseB(adId, false)
                        _bridge.callCpp(kOnClosed, response.serialize())
                        return
                    }
                    if (state == FinishState.COMPLETED) {
                        @Serializable
                        @Suppress("unused")
                        class ResponseC(
                            val ad_id: String,
                            val rewarded: Boolean
                        )

                        _loadedAdIds.remove(adId)
                        val response = ResponseC(adId, true)
                        _bridge.callCpp(kOnClosed, response.serialize())
                        return
                    }
                    assertThat(false).isTrue()
                }

                override fun onUnityAdsError(unityAdsError: UnityAdsError, message: String) {
                    _logger.info("${this::onUnityAdsError.name}: error = $unityAdsError message = $message")
                    Thread.checkMainThread()
                }
            }
            UnityAds.initialize(_activity, gameId, testModeEnabled)
            UnityAds.addListener(_listener)
            _initialized = true
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
    fun showRewardedAd(adId: String) {
        Thread.runOnMainThread {
            if (!_initialized) {
                throw IllegalStateException("Please call initialize() first")
            }
            UnityAds.show(_activity, adId)
        }
    }
}