package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.internal.AdMobBannerAd
import com.ee.internal.AdMobBannerHelper
import com.ee.internal.AdMobInterstitialAd
import com.ee.internal.AdMobNativeAd
import com.ee.internal.AdMobRewardedAd
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.AdSize
import com.google.android.gms.ads.MobileAds
import com.google.android.gms.ads.RequestConfiguration
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.util.concurrent.ConcurrentHashMap

/**
 * Created by Zinge on 10/13/17.
 */
@InternalSerializationApi
class AdMobBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = AdMobBridge::class.java.name
        private const val kPrefix = "AdMobBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kGetEmulatorTestDeviceHash = "${kPrefix}GetEmulatorTestDeviceHash"
        private const val kAddTestDevice = "${kPrefix}AddTestDevice"
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

    private val _bannerHelper = AdMobBannerHelper(_context)
    private val _testDevices: MutableList<String> = ArrayList()
    private val _bannerAds: MutableMap<String, AdMobBannerAd> = ConcurrentHashMap()
    private val _nativeAds: MutableMap<String, AdMobNativeAd> = ConcurrentHashMap()
    private val _interstitialAds: MutableMap<String, AdMobInterstitialAd> = ConcurrentHashMap()
    private val _rewardedAds: MutableMap<String, AdMobRewardedAd> = ConcurrentHashMap()

    init {
        _logger.info("$kTag: constructor begin: context = $_context")
        registerHandlers()
        _logger.info("$kTag: constructor end.")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        for (ad in _bannerAds.values) {
            ad.onCreate(activity)
        }
        for (ad in _nativeAds.values) {
            ad.onCreate(activity)
        }
        for (ad in _rewardedAds.values) {
            ad.onCreate(activity)
        }
    }

    override fun onStart() {}
    override fun onStop() {}

    override fun onResume() {
        for (ad in _bannerAds.values) {
            ad.onResume()
        }
    }

    override fun onPause() {
        for (ad in _bannerAds.values) {
            ad.onPause()
        }
    }

    override fun onDestroy() {
        val activity = _activity ?: return
        for (ad in _bannerAds.values) {
            ad.onDestroy(activity)
        }
        for (ad in _nativeAds.values) {
            ad.onDestroy(activity)
        }
        for (ad in _rewardedAds.values) {
            ad.onDestroy(activity)
        }
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
        _bridge.registerHandler(kInitialize) {
            initialize()
            ""
        }
        _bridge.registerHandler(kGetEmulatorTestDeviceHash) {
            emulatorTestDeviceHash
        }
        _bridge.registerHandler(kAddTestDevice) { message ->
            addTestDevice(message)
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
        _bridge.deregisterHandler(kGetEmulatorTestDeviceHash)
        _bridge.deregisterHandler(kAddTestDevice)
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

    @AnyThread
    fun initialize() {
        Thread.runOnMainThread {
            MobileAds.initialize(_context)
        }
    }

    private val emulatorTestDeviceHash: String
        @AnyThread get() = AdRequest.DEVICE_ID_EMULATOR

    @AnyThread
    fun addTestDevice(hash: String) {
        Thread.runOnMainThread {
            _testDevices.add(hash)
            val configuration = RequestConfiguration.Builder()
                .setTestDeviceIds(_testDevices)
                .build()
            MobileAds.setRequestConfiguration(configuration)
        }
    }

    @AnyThread
    fun createBannerAd(adId: String, adSize: AdSize): Boolean {
        if (_bannerAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobBannerAd(_bridge, _logger, _context, _activity, adId, adSize, _bannerHelper)
        _bannerAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyBannerAd(adId: String): Boolean {
        val ad = _bannerAds[adId] ?: return false
        ad.destroy()
        _bannerAds.remove(adId)
        return true
    }

    @AnyThread
    fun createNativeAd(adId: String, layoutName: String,
                       identifiers: Map<String, String>): Boolean {
        if (_nativeAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobNativeAd(_bridge, _logger, _context, _activity, adId, layoutName, identifiers)
        _nativeAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyNativeAd(adId: String): Boolean {
        val ad = _nativeAds[adId] ?: return false
        ad.destroy()
        _nativeAds.remove(adId)
        return true
    }

    @AnyThread
    fun createInterstitialAd(adId: String): Boolean {
        if (_interstitialAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobInterstitialAd(_bridge, _logger, _context, adId)
        _interstitialAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyInterstitialAd(adId: String): Boolean {
        val ad = _interstitialAds[adId] ?: return false
        ad.destroy()
        _interstitialAds.remove(adId)
        return true
    }

    @AnyThread
    fun createRewardedAd(adId: String): Boolean {
        if (_rewardedAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobRewardedAd(_bridge, _logger, _context, _activity, adId)
        _rewardedAds[adId] = ad
        return true
    }

    @AnyThread
    fun destroyRewardedAd(adId: String): Boolean {
        val ad = _rewardedAds[adId] ?: return false
        ad.destroy()
        _rewardedAds.remove(adId)
        return true
    }
}