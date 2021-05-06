package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.AdMobAppOpenAd
import com.ee.internal.AdMobBannerAd
import com.ee.internal.AdMobBannerHelper
import com.ee.internal.AdMobInterstitialAd
import com.ee.internal.AdMobRewardedAd
import com.ee.internal.AdMobRewardedInterstitialAd
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.google.android.ads.mediationtestsuite.MediationTestSuite
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.AdSize
import com.google.android.gms.ads.MobileAds
import com.google.android.gms.ads.RequestConfiguration
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.Serializable
import java.util.concurrent.ConcurrentHashMap
import kotlin.coroutines.resume

/**
 * Created by Zinge on 10/13/17.
 */
class AdMobBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = AdMobBridge::class.java.name
        private const val kPrefix = "AdMobBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kGetEmulatorTestDeviceHash = "${kPrefix}GetEmulatorTestDeviceHash"
        private const val kAddTestDevice = "${kPrefix}AddTestDevice"
        private const val kOpenTestSuite = "${kPrefix}OpenTestSuite"
        private const val kGetBannerAdSize = "${kPrefix}GetBannerAdSize"
        private const val kCreateBannerAd = "${kPrefix}CreateBannerAd"
        private const val kCreateNativeAd = "${kPrefix}CreateNativeAd"
        private const val kCreateAppOpenAd = "${kPrefix}CreateAppOpenAd"
        private const val kCreateInterstitialAd = "${kPrefix}CreateInterstitialAd"
        private const val kCreateRewardedInterstitialAd = "${kPrefix}CreateRewardedInterstitialAd"
        private const val kCreateRewardedAd = "${kPrefix}CreateRewardedAd"
        private const val kDestroyAd = "${kPrefix}DestroyAd"
    }

    private var _initializing = false
    private var _initialized = false
    private val _bannerHelper = AdMobBannerHelper(_application)
    private val _testDevices: MutableList<String> = ArrayList()
    private val _ads: MutableMap<String, IAd> = ConcurrentHashMap()

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

    @Serializable
    private class CreateNativeAdRequest(
        val adId: String,
        val layoutName: String,
        val identifiers: Map<String, String>
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) {
            Utils.toString(initialize())
        }
        _bridge.registerHandler(kGetEmulatorTestDeviceHash) {
            emulatorTestDeviceHash
        }
        _bridge.registerHandler(kAddTestDevice) { message ->
            addTestDevice(message)
            ""
        }
        _bridge.registerHandler(kOpenTestSuite) {
            openTestSuite()
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
        _bridge.registerHandler(kCreateNativeAd) { message ->
            val request = deserialize<CreateNativeAdRequest>(message)
            Utils.toString(createNativeAd(request.adId, request.layoutName, request.identifiers))
        }
        _bridge.registerHandler(kCreateAppOpenAd) { message ->
            Utils.toString(createAppOpenAd(message))
        }
        _bridge.registerHandler(kCreateInterstitialAd) { message ->
            Utils.toString(createInterstitialAd(message))
        }
        _bridge.registerHandler(kCreateRewardedInterstitialAd) { message ->
            Utils.toString(createRewardedInterstitialAd(message))
        }
        _bridge.registerHandler(kCreateRewardedAd) { message ->
            Utils.toString(createRewardedAd(message))
        }
        _bridge.registerHandler(kDestroyAd) { message ->
            Utils.toString(destroyAd(message))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kGetEmulatorTestDeviceHash)
        _bridge.deregisterHandler(kAddTestDevice)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kCreateNativeAd)
        _bridge.deregisterHandler(kCreateAppOpenAd)
        _bridge.deregisterHandler(kCreateInterstitialAd)
        _bridge.deregisterHandler(kCreateRewardedInterstitialAd)
        _bridge.deregisterHandler(kCreateRewardedAd)
        _bridge.deregisterHandler(kDestroyAd)
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
                val activity = _activity
                if (activity == null) {
                    cont.resume(true)
                    return@runOnMainThread
                }
                _initializing = true
                MobileAds.initialize(activity) innerInitialize@{ status ->
                    if (cont.isActive) {
                        // OK.
                    } else {
                        return@innerInitialize
                    }
                    Thread.runOnMainThread {
                        _initializing = false
                        _initialized = true
                        _logger.info("$kTag: initialize: done")
                        for ((key, value) in status.adapterStatusMap) {
                            _logger.info("$kTag: adapter = $key state = ${value.initializationState} latency = ${value.latency} description = ${value.description}")
                        }
                        cont.resume(true)
                    }
                }
            }
        }
    }

    private val emulatorTestDeviceHash: String
        @AnyThread get() = AdRequest.DEVICE_ID_EMULATOR

    @AnyThread
    fun addTestDevice(hash: String) {
        Thread.runOnMainThread {
            checkInitialized()
            _testDevices.add(hash)
            val configuration = RequestConfiguration.Builder()
                .setTestDeviceIds(_testDevices)
                .build()
            MobileAds.setRequestConfiguration(configuration)
        }
    }

    @AnyThread
    fun openTestSuite() {
        Thread.runOnMainThread {
            val activity = _activity ?: return@runOnMainThread
            MediationTestSuite.launch(activity)
        }
    }

    @AnyThread
    fun createBannerAd(adId: String, adSize: AdSize): Boolean {
        return createAd(adId) {
            AdMobBannerAd(_bridge, _logger, _activity, adId, adSize, _bannerHelper)
        }
    }

    @AnyThread
    fun createNativeAd(adId: String, layoutName: String,
                       identifiers: Map<String, String>): Boolean {
        return false
        /* FIXME
        return createAd(adId) {
            AdMobNativeAd(_bridge, _logger, _application, _activity, adId, layoutName, identifiers)
        }
         */
    }

    @AnyThread
    fun createAppOpenAd(adId: String): Boolean {
        return createAd(adId) {
            AdMobAppOpenAd(_bridge, _logger, _application, _activity, adId)
        }
    }

    @AnyThread
    fun createInterstitialAd(adId: String): Boolean {
        return createAd(adId) {
            AdMobInterstitialAd(_bridge, _logger, _activity, adId)
        }
    }

    @AnyThread
    fun createRewardedInterstitialAd(adId: String): Boolean {
        return createAd(adId) {
            AdMobRewardedInterstitialAd(_bridge, _logger, _activity, adId)
        }
    }

    @AnyThread
    fun createRewardedAd(adId: String): Boolean {
        return createAd(adId) {
            AdMobRewardedAd(_bridge, _logger, _activity, adId)
        }
    }

    @AnyThread
    fun createAd(adId: String, creator: () -> IAd): Boolean {
        checkInitialized()
        if (_ads.containsKey(adId)) {
            return false
        }
        val ad = creator()
        _ads[adId] = ad
        return true
    }

    @AnyThread
    fun destroyAd(adId: String): Boolean {
        checkInitialized()
        val ad = _ads[adId] ?: return false
        ad.destroy()
        _ads.remove(adId)
        return true
    }
}