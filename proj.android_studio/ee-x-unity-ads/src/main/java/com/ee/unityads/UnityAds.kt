package com.ee.unityads

import android.app.Activity
import android.content.Context
import android.content.Intent
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.internal.serialize
import com.ee.core.registerHandler
import com.google.common.truth.Truth.assertThat
import com.unity3d.ads.IUnityAdsListener
import com.unity3d.ads.UnityAds.FinishState
import com.unity3d.ads.UnityAds.UnityAdsError
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

@ImplicitReflectionSerializer
@UnstableDefault
class UnityAds(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(UnityAds::class.java.name)

        private const val kPrefix = "UnityAds"
        private const val k__initialize = "${kPrefix}_initialize"
        private const val k__setDebugModeEnabled = "${kPrefix}_setDebugModeEnabled"
        private const val k__hasRewardedAd = "${kPrefix}_hasRewardedAd"
        private const val k__showRewardedAd = "${kPrefix}_showRewardedAd"
        private const val k__onLoaded = "${kPrefix}_onLoaded"
        private const val k__onFailedToShow = "${kPrefix}_onFailedToShow"
        private const val k__onClosed = "${kPrefix}_onClosed"
    }

    private var _initialized = false
    private var _listener: IUnityAdsListener? = null

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

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        return false
    }

    override fun destroy() {
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            if (!_initialized) {
                return@Runnable
            }
            com.unity3d.ads.UnityAds.removeListener(_listener)
        })
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__initialize) { message ->
            @Serializable
            class Request(
                val gameId: String,
                val testModeEnabled: Boolean
            )

            val request = deserialize<Request>(message)
            initialize(request.gameId, request.testModeEnabled)
            ""
        }
        _bridge.registerHandler(k__setDebugModeEnabled) { message ->
            setDebugModeEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__hasRewardedAd) { message ->
            Utils.toString(hasRewardedAd(message))
        }
        _bridge.registerHandler(k__showRewardedAd) { message ->
            showRewardedAd(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize)
        _bridge.deregisterHandler(k__setDebugModeEnabled)
        _bridge.deregisterHandler(k__hasRewardedAd)
        _bridge.deregisterHandler(k__showRewardedAd)
    }

    @UiThread
    fun initialize(gameId: String, testModeEnabled: Boolean) {
        Thread.checkMainThread()
        if (_initialized) {
            return
        }
        if (!com.unity3d.ads.UnityAds.isSupported()) {
            return
        }
        _listener = object : IUnityAdsListener {
            override fun onUnityAdsReady(adId: String) {
                _logger.info("${this::onUnityAdsReady}: $adId")
                Thread.checkMainThread()
                _bridge.callCpp(k__onLoaded, adId)
            }

            override fun onUnityAdsStart(adId: String) {
                _logger.info("${this::onUnityAdsStart}: $adId")
                Thread.checkMainThread()
            }

            override fun onUnityAdsFinish(adId: String, state: FinishState) {
                _logger.info("${this::onUnityAdsFinish}: $adId state = $state")
                Thread.checkMainThread()
                if (state == FinishState.ERROR) {
                    @Serializable
                    @Suppress("unused")
                    class ResponseA(
                        val ad_id: String,
                        val message: String
                    )

                    val response = ResponseA(adId, "")
                    _bridge.callCpp(k__onFailedToShow, response.serialize())
                    return
                }
                if (state == FinishState.SKIPPED) {
                    @Serializable
                    @Suppress("unused")
                    class ResponseB(
                        val ad_id: String,
                        val rewarded: Boolean
                    )

                    val response = ResponseB(adId, false)
                    _bridge.callCpp(k__onClosed, response.serialize())
                    return
                }
                if (state == FinishState.COMPLETED) {
                    @Serializable
                    @Suppress("unused")
                    class ResponseC(
                        val ad_id: String,
                        val rewarded: Boolean
                    )

                    val response = ResponseC(adId, true)
                    _bridge.callCpp(k__onClosed, response.serialize())
                    return
                }
                assertThat(false).isTrue()
            }

            override fun onUnityAdsError(unityAdsError: UnityAdsError, s: String) {
                _logger.info("${this::onUnityAdsError}: $s error = $unityAdsError")
                Thread.checkMainThread()
            }
        }
        com.unity3d.ads.UnityAds.initialize(_activity, gameId, testModeEnabled)
        com.unity3d.ads.UnityAds.addListener(_listener)
        _initialized = true
    }

    @UiThread
    private fun setDebugModeEnabled(enabled: Boolean) {
        Thread.checkMainThread()
        if (!_initialized) {
            return
        }
        com.unity3d.ads.UnityAds.setDebugMode(enabled)
    }

    @UiThread
    fun hasRewardedAd(adId: String): Boolean {
        Thread.checkMainThread()
        if (!_initialized) {
            throw IllegalStateException("Please call initialize() first")
        }
        return com.unity3d.ads.UnityAds.isReady(adId)
    }

    @UiThread
    fun showRewardedAd(adId: String) {
        Thread.checkMainThread()
        if (!_initialized) {
            throw IllegalStateException("Please call initialize() first")
        }
        com.unity3d.ads.UnityAds.show(_activity, adId)
    }
}