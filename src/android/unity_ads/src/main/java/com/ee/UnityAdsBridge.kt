package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.UnityBannerAd
import com.ee.internal.UnityBannerHelper
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.google.common.truth.Truth.assertThat
import com.unity3d.ads.IUnityAdsInitializationListener
import com.unity3d.ads.IUnityAdsListener
import com.unity3d.ads.IUnityAdsLoadListener
import com.unity3d.ads.UnityAds
import com.unity3d.ads.UnityAds.FinishState
import com.unity3d.ads.UnityAds.UnityAdsError
import com.unity3d.services.banners.UnityBannerSize
import kotlinx.coroutines.suspendCancellableCoroutine
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
        _bridge.registerHandler(kGetBannerAdSize) { message ->
            @Serializable
            @Suppress("unused")
            class Response(
                val width: Int,
                val height: Int
            )

            val index = message.toInt()
            val size = _bannerHelper.getSize(index)
            val response = Response(size.x, size.y)
            response.serialize()
        }
        _bridge.registerHandler(kCreateBannerAd) { message ->
            @Serializable
            class Request(
                val adId: String,
                val adSize: Int
            )

            val request = deserialize<Request>(message)
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
                UnityAds.initialize(_application, gameId, testModeEnabled, true, object : IUnityAdsInitializationListener {
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

                    override fun onInitializationFailed(error: UnityAds.UnityAdsInitializationError?, message: String?) {
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
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                _logger.debug("$kTag: loadRewardedAd: id = $adId")
                if (!_initialized) {
                    _logger.error("$kTag: loadRewardedAd: not initialized")
                    cont.resume(false)
                    return@runOnMainThread
                }
                UnityAds.load(adId, object : IUnityAdsLoadListener {
                    override fun onUnityAdsAdLoaded(placementId: String) {
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: ${this::onUnityAdsAdLoaded.name}: id = $adId")
                            _loadedAdIds.add(adId)
                            cont.resume(true)
                        }
                    }

                    override fun onUnityAdsFailedToLoad(placementId: String) {
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: ${this::onUnityAdsFailedToLoad.name}: id = $adId")
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
            _logger.debug("$kTag: ${this::showRewardedAd.name}: id = $adId")
            if (!_initialized) {
                _logger.error("$kTag: ${this::showRewardedAd.name}: not initialized")
                @Serializable
                @Suppress("unused")
                class ResponseA(
                    val ad_id: String,
                    val message: String
                )

                val response = ResponseA(adId, "not initialized")
                _bridge.callCpp(kOnFailedToShow, response.serialize())
                return@runOnMainThread
            }
            UnityAds.addListener(this)
            UnityAds.show(_activity, adId)
        }
    }

    override fun onUnityAdsReady(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsReady.name}: id = $adId")
            _loadedAdIds.add(adId)
            _bridge.callCpp(kOnLoaded, adId)
        }
    }

    override fun onUnityAdsStart(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsStart.name}: id = $adId")
        }
    }

    override fun onUnityAdsFinish(adId: String, state: FinishState) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onUnityAdsFinish.name}: id = $adId state = $state")
            UnityAds.removeListener(this)
            _loadedAdIds.remove(adId)
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