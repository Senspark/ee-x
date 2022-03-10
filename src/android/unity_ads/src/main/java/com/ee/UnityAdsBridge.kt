package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.UnityBannerAd
import com.ee.internal.UnityBannerHelper
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.unity3d.ads.IUnityAdsInitializationListener
import com.unity3d.ads.IUnityAdsLoadListener
import com.unity3d.ads.IUnityAdsShowListener
import com.unity3d.ads.UnityAds
import com.unity3d.ads.UnityAds.UnityAdsShowCompletionState
import com.unity3d.services.banners.UnityBannerSize
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.Serializable
import java.util.Collections
import java.util.concurrent.ConcurrentHashMap
import kotlin.coroutines.resume

class UnityAdsBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = UnityAdsBridge::class.java.name
        private const val kPrefix = "UnityAdsBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetDebugModeEnabled = "${kPrefix}SetDebugModeEnabled"
        private const val kGetBannerAdSize = "${kPrefix}GetBannerAdSize"
        private const val kCreateBannerAd = "${kPrefix}CreateBannerAd"
        private const val kDestroyAd = "${kPrefix}DestroyAd"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kLoadRewardedAd = "${kPrefix}LoadRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
        private const val kOnLoaded = "${kPrefix}OnLoaded"
        private const val kOnFailedToShow = "${kPrefix}OnFailedToShow"
        private const val kOnClosed = "${kPrefix}OnClosed"
    }

    private var _initializing = false
    private var _initialized = false
    private val _bannerHelper = UnityBannerHelper()
    private val _ads: MutableMap<String, IAd> = ConcurrentHashMap()
    private val _loadedAdIds: MutableSet<String> = Collections.newSetFromMap(ConcurrentHashMap())

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        for (ad in _ads.values) {
            ad.onCreate(activity)
        }
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {
        for (ad in _ads.values) {
            ad.onResume()
        }
    }

    override fun onPause() {
        for (ad in _ads.values) {
            ad.onPause()
        }
    }

    override fun onDestroy() {
        for (ad in _ads.values) {
            ad.onDestroy()
        }
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
        for (ad in _ads.values) {
            ad.destroy()
        }
        _ads.clear()
    }

    @Serializable
    private class InitializeRequest(
        val gameId: String,
        val testModeEnabled: Boolean
    )

    @Serializable
    @Suppress("unused")
    private class GetBannerAdSizeResponse(
        val width: Int,
        val height: Int
    )

    @Serializable
    private class CreateBannerAdRequest(
        val adId: String,
        val adSize: Int
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            val request = deserialize<InitializeRequest>(message)
            Utils.toString(initialize(request.gameId, request.testModeEnabled))
        }
        _bridge.registerHandler(kSetDebugModeEnabled) { message ->
            setDebugModeEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kGetBannerAdSize) { message ->
            val index = message.toInt()
            val size = _bannerHelper.getSize(index)
            val response = GetBannerAdSizeResponse(size.x, size.y)
            response.serialize()
        }
        _bridge.registerHandler(kCreateBannerAd) { message ->
            val request = deserialize<CreateBannerAdRequest>(message)
            val adSize = _bannerHelper.getAdSize(request.adSize)
            Utils.toString(createBannerAd(request.adId, adSize))
        }
        _bridge.registerHandler(kDestroyAd) { message ->
            Utils.toString(destroyAd(message))
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
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kDestroyAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    @AnyThread
    suspend fun initialize(gameId: String, testModeEnabled: Boolean): Boolean {
        return suspendCancellableCoroutine { cont ->
            Thread.runOnMainThread {
                if (!UnityAds.isSupported()) {
                    _logger.info("$kTag: initialize: not supported")
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (UnityAds.isInitialized()) {
                    _initialized = true
                    _logger.info("$kTag: initialize: initialized")
                    cont.resume(true)
                    return@runOnMainThread
                }
                if (_initializing) {
                    _logger.info("$kTag: initialize: initializing")
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (_initialized) {
                    _logger.info("$kTag: initialize: initialized")
                    cont.resume(true)
                    return@runOnMainThread
                }
                _initializing = true
                UnityAds.initialize(
                    _application,
                    gameId,
                    testModeEnabled,
                    object : IUnityAdsInitializationListener {
                        override fun onInitializationComplete() {
                            if (cont.isActive) {
                                // OK.
                            } else {
                                return
                            }
                            Thread.runOnMainThread {
                                _logger.info("$kTag: initialize: done")
                                _initializing = false
                                _initialized = true
                                cont.resume(true)
                            }
                        }

                        override fun onInitializationFailed(
                            error: UnityAds.UnityAdsInitializationError?,
                            message: String?
                        ) {
                            if (cont.isActive) {
                                // OK.
                            } else {
                                return
                            }
                            Thread.runOnMainThread {
                                _logger.error("${this::onInitializationFailed.name}: error = ${error ?: ""} message = ${message ?: ""}")
                                _initializing = false
                                _initialized = true
                                cont.resume(true)
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
                return@runOnMainThread
            }
            UnityAds.setDebugMode(enabled)
        }
    }

    @AnyThread
    fun createBannerAd(adId: String, adSize: UnityBannerSize): Boolean {
        return createAd(adId) {
            UnityBannerAd(_bridge, _logger, _activity, adId, adSize, _bannerHelper)
        }
    }

    @AnyThread
    fun createAd(adId: String, creator: () -> IAd): Boolean {
        if (_ads.containsKey(adId)) {
            return false
        }
        val ad = creator()
        _ads[adId] = ad
        return true
    }

    @AnyThread
    fun destroyAd(adId: String): Boolean {
        val ad = _ads[adId] ?: return false
        ad.destroy()
        _ads.remove(adId)
        return true
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
        return suspendCancellableCoroutine { cont ->
            Thread.runOnMainThread {
                _logger.debug("$kTag: loadRewardedAd: id = $adId")
                if (!_initialized) {
                    _logger.error("$kTag: loadRewardedAd: not initialized")
                    cont.resume(false)
                    return@runOnMainThread
                }
                UnityAds.load(adId, object : IUnityAdsLoadListener {
                    override fun onUnityAdsAdLoaded(placementId: String) {
                        if (cont.isActive) {
                            // OK.
                        } else {
                            return
                        }
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: ${this::onUnityAdsAdLoaded.name}: id = $adId")
                            _loadedAdIds.add(adId)
                            cont.resume(true)
                        }
                    }

                    override fun onUnityAdsFailedToLoad(
                        placementId: String,
                        error: UnityAds.UnityAdsLoadError,
                        message: String
                    ) {
                        if (cont.isActive) {
                            // OK.
                        } else {
                            return
                        }
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: ${this::onUnityAdsFailedToLoad.name}: id = $adId")
                            cont.resume(false)
                        }
                    }
                })
            }
        }
    }

    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val ad_id: String,
        val message: String
    )

    @Serializable
    @Suppress("unused")
    private class ResultResponse(
        val ad_id: String,
        val rewarded: Boolean
    )

    @AnyThread
    fun showRewardedAd(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::showRewardedAd.name}: id = $adId")
            if (!_initialized) {
                _logger.error("$kTag: ${this::showRewardedAd.name}: not initialized")
                val response = ErrorResponse(adId, "not initialized")
                _bridge.callCpp(kOnFailedToShow, response.serialize())
                return@runOnMainThread
            }
            UnityAds.show(_activity, adId, object : IUnityAdsShowListener {
                override fun onUnityAdsShowStart(placementId: String?) {
                    _logger.debug("$kTag: ${this::onUnityAdsShowStart.name}: placementId = $placementId")
                }

                override fun onUnityAdsShowClick(placementId: String?) {
                    _logger.debug("$kTag: ${this::onUnityAdsShowClick.name}: placementId = $placementId")
                }

                override fun onUnityAdsShowComplete(
                    placementId: String?,
                    state: UnityAdsShowCompletionState?
                ) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onUnityAdsShowComplete.name}: placementId = $placementId state = $state")
                        _loadedAdIds.remove(placementId)
                        val response =
                            ResultResponse(adId, state == UnityAdsShowCompletionState.COMPLETED)
                        _bridge.callCpp(kOnClosed, response.serialize())
                    }
                }

                override fun onUnityAdsShowFailure(
                    placementId: String?,
                    error: UnityAds.UnityAdsShowError?,
                    message: String?
                ) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onUnityAdsShowFailure.name}: placementId = $placementId message = $message")
                        _loadedAdIds.remove(placementId)
                        val response = ErrorResponse(adId, "")
                        _bridge.callCpp(kOnFailedToShow, response.serialize())
                    }
                }
            })
        }
    }
}