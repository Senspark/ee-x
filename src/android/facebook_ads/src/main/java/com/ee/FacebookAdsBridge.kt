package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.facebook.ads.BuildConfig
import com.ee.internal.FacebookBannerAd
import com.ee.internal.FacebookBannerHelper
import com.ee.internal.FacebookInterstitialAd
import com.ee.internal.FacebookNativeAd
import com.ee.internal.FacebookRewardedAd
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.facebook.ads.AdSettings
import com.facebook.ads.AdSize
import com.facebook.ads.AudienceNetworkAds
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.util.concurrent.ConcurrentHashMap
import kotlin.coroutines.resume

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@InternalSerializationApi
class FacebookAdsBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = FacebookAdsBridge::class.java.name
        private const val kPrefix = "FacebookAdsBridge"
        private const val kInitialize = "${kPrefix}Initialize";
        private const val kGetTestDeviceHash = "${kPrefix}GetTestDeviceHash"
        private const val kAddTestDevice = "${kPrefix}AddTestDevice"
        private const val kClearTestDevices = "${kPrefix}ClearTestDevices"
        private const val kGetBannerAdSize = "${kPrefix}GetBannerAdSize"
        private const val kCreateBannerAd = "${kPrefix}CreateBannerAd"
        private const val kDestroyBannerAd = "${kPrefix}DestroyBannerAd"
        private const val kCreateNativeAd = "${kPrefix}CreateNativeAd"
        private const val kDestroyNativeAd = "${kPrefix}DestroyNativeAd"
        private const val kCreateInterstitialAd = "${kPrefix}CreateInterstitialAd"
        private const val kDestroyInterstitialAd = "${kPrefix}DestroyInterstitialAd"
        private const val kCreateRewardedAd = "${kPrefix}CreateRewardedAd"
        private const val kDestroyRewardedAd = "${kPrefix}DestroyRewardedAd"
    }

    private var _initializing = false
    private var _initialized = false
    private val _bannerHelper = FacebookBannerHelper()
    private val _bannerAds: MutableMap<String, FacebookBannerAd> = ConcurrentHashMap()
    private val _nativeAds: MutableMap<String, FacebookNativeAd> = ConcurrentHashMap()
    private val _interstitialAds: MutableMap<String, FacebookInterstitialAd> = ConcurrentHashMap()
    private val _rewardedAds: MutableMap<String, FacebookRewardedAd> = ConcurrentHashMap()

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        for (ad in _bannerAds.values) {
            ad.onCreate(activity)
        }
        for (ad in _nativeAds.values) {
            ad.onCreate(activity)
        }
        for (ad in _interstitialAds.values) {
            ad.onCreate(activity)
        }
        for (ad in _rewardedAds.values) {
            ad.onCreate(activity)
        }
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        val activity = _activity ?: return
        for (ad in _bannerAds.values) {
            ad.onDestroy(activity)
        }
        for (ad in _nativeAds.values) {
            ad.onDestroy(activity)
        }
        for (ad in _interstitialAds.values) {
            ad.onDestroy(activity)
        }
        for (ad in _rewardedAds.values) {
            ad.onDestroy(activity)
        }
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
        for (ad in _bannerAds.values) {
            ad.destroy()
        }
        _bannerAds.clear()
        for (ad in _nativeAds.values) {
            ad.destroy()
        }
        _nativeAds.clear()
        for (ad in _interstitialAds.values) {
            ad.destroy()
        }
        _interstitialAds.clear()
        for (ad in _rewardedAds.values) {
            ad.destroy()
        }
        _rewardedAds.clear()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) {
            Utils.toString(initialize())
        }
        _bridge.registerHandler(kGetTestDeviceHash) {
            testDeviceHash
        }
        _bridge.registerHandler(kAddTestDevice) { message ->
            addTestDevice(message)
            ""
        }
        _bridge.registerHandler(kClearTestDevices) {
            clearTestDevices()
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
        _bridge.registerHandler(kDestroyBannerAd) { message ->
            Utils.toString(destroyBannerAd(message))
        }
        _bridge.registerHandler(kCreateNativeAd) { message ->
            @Serializable
            class Request(
                val adId: String,
                val layoutName: String,
                val identifiers: Map<String, String>
            )

            val request = deserialize<Request>(message)
            Utils.toString(createNativeAd(request.adId, request.layoutName, request.identifiers))
        }
        _bridge.registerHandler(kDestroyNativeAd) { message ->
            Utils.toString(destroyNativeAd(message))
        }
        _bridge.registerHandler(kCreateInterstitialAd) { message ->
            Utils.toString(createInterstitialAd(message))
        }
        _bridge.registerHandler(kDestroyInterstitialAd) { message ->
            Utils.toString(destroyInterstitialAd(message))
        }
        _bridge.registerHandler(kCreateRewardedAd) { message ->
            Utils.toString(createRewardedAd(message))
        }
        _bridge.registerHandler(kDestroyRewardedAd) { message ->
            Utils.toString(destroyRewardedAd(message))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kGetTestDeviceHash)
        _bridge.deregisterHandler(kAddTestDevice)
        _bridge.deregisterHandler(kClearTestDevices)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kDestroyBannerAd)
        _bridge.deregisterHandler(kCreateNativeAd)
        _bridge.deregisterHandler(kDestroyNativeAd)
        _bridge.deregisterHandler(kCreateInterstitialAd)
        _bridge.deregisterHandler(kDestroyInterstitialAd)
        _bridge.deregisterHandler(kCreateRewardedAd)
        _bridge.deregisterHandler(kDestroyRewardedAd)
    }

    private fun checkInitialized() {
        if (!_initialized) {
            throw IllegalStateException("Please call initialize() first")
        }
    }

    @AnyThread
    suspend fun initialize(): Boolean {
        return suspendCancellableCoroutine { cont ->
            Thread.runOnMainThread {
                if (AudienceNetworkAds.isInitialized(_application)) {
                    _logger.info("$kTag: initialize: initialized")
                    _initialized = true
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
                AudienceNetworkAds
                    .buildInitSettings(_application)
                    .withInitListener { result ->
                        if (cont.isActive) {
                            // OK.
                        } else {
                            return@withInitListener
                        }
                        Thread.runOnMainThread {
                            _logger.info("$kTag: initialize: result = ${result.isSuccess} message = ${result.message ?: ""}")
                            _initializing = false
                            _initialized = true
                            if (result.isSuccess) {
                                if (BuildConfig.DEBUG) {
                                    AdSettings.setDebugBuild(true)
                                }
                                AdSettings.setTestMode(false)
                            } else {
                                _logger.error("$kTag: initialize: result = $result")
                            }
                            cont.resume(true)
                        }
                    }
                    .initialize()
            }
        }
    }

    private val testDeviceHash: String
        @AnyThread get() = ""

    @AnyThread
    fun addTestDevice(hash: String) {
        Thread.runOnMainThread {
            checkInitialized()
            AdSettings.addTestDevice(hash)
        }
    }

    @AnyThread
    fun clearTestDevices() {
        Thread.runOnMainThread {
            checkInitialized()
            AdSettings.clearTestDevices()
        }
    }

    @AnyThread
    fun createBannerAd(adId: String, adSize: AdSize): Boolean {
        checkInitialized()
        if (_bannerAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookBannerAd(_bridge, _logger, _activity, adId, adSize, _bannerHelper)
        _bannerAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyBannerAd(adId: String): Boolean {
        checkInitialized()
        val ad = _bannerAds[adId] ?: return false
        ad.destroy()
        _bannerAds.remove(adId)
        return true
    }

    @AnyThread
    fun createNativeAd(adId: String, layoutName: String,
                       identifiers: Map<String, String>): Boolean {
        checkInitialized()
        if (_nativeAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookNativeAd(_bridge, _logger, _application, _activity, adId, layoutName, identifiers)
        _nativeAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyNativeAd(adId: String): Boolean {
        checkInitialized()
        val ad = _nativeAds[adId] ?: return false
        ad.destroy()
        _nativeAds.remove(adId)
        return true
    }

    @AnyThread
    private fun createInterstitialAd(adId: String): Boolean {
        checkInitialized()
        if (_interstitialAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookInterstitialAd(_bridge, _logger, _activity, adId)
        _interstitialAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyInterstitialAd(adId: String): Boolean {
        checkInitialized()
        val ad = _interstitialAds[adId] ?: return false
        ad.destroy()
        _interstitialAds.remove(adId)
        return true
    }

    @AnyThread
    private fun createRewardedAd(adId: String): Boolean {
        checkInitialized()
        if (_rewardedAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookRewardedAd(_bridge, _logger, _activity, adId)
        _rewardedAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyRewardedAd(adId: String): Boolean {
        checkInitialized()
        val ad = _rewardedAds[adId] ?: return false
        ad.destroy()
        _rewardedAds.remove(adId)
        return true
    }
}