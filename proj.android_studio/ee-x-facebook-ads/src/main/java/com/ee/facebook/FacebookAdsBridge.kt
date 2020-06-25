package com.ee.facebook

import android.app.Activity
import android.content.Context
import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.internal.serialize
import com.ee.core.registerHandler
import com.ee.facebook.ads.BuildConfig
import com.facebook.ads.AdSettings
import com.facebook.ads.AdSize
import com.facebook.ads.AudienceNetworkAds
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault
import java.util.concurrent.ConcurrentHashMap

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
class FacebookAdsBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private const val kPrefix = "FacebookAdsBridge"
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

    private val _bannerHelper = FacebookBannerHelper()
    private val _bannerAds: MutableMap<String, FacebookBannerAd> = ConcurrentHashMap()
    private val _nativeAds: MutableMap<String, FacebookNativeAd> = ConcurrentHashMap()
    private val _interstitialAds: MutableMap<String, FacebookInterstitialAd> = ConcurrentHashMap()
    private val _rewardedAds: MutableMap<String, FacebookRewardedAd> = ConcurrentHashMap()

    init {
        if (!AudienceNetworkAds.isInitialized(_context)) {
            if (BuildConfig.DEBUG) {
                AdSettings.setDebugBuild(true)
            }
            AudienceNetworkAds.initialize(_context)
            AudienceNetworkAds.isInAdsProcess(_context)
        }
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        for (ad in _bannerAds.values) {
            ad.onCreate(activity)
        }
        for (ad in _nativeAds.values) {
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

            val sizeId = message.toInt()
            val size = getBannerAdSize(sizeId)
            val response = Response(size.x, size.y)
            response.serialize()
        }
        _bridge.registerHandler(kCreateBannerAd) { message ->
            @Serializable
            class Request(
                val ad_id: String,
                val ad_size: Int
            )

            val request = deserialize<Request>(message)
            val adSize = _bannerHelper.getAdSize(request.ad_size)
            Utils.toString(createBannerAd(request.ad_id, adSize))
        }
        _bridge.registerHandler(kDestroyBannerAd) { message ->
            Utils.toString(destroyBannerAd(message))
        }
        _bridge.registerHandler(kCreateNativeAd) { message ->
            @Serializable
            class Request(
                val ad_id: String,
                val layout_name: String,
                val identifiers: Map<String, String>
            )

            val request = deserialize<Request>(message)
            Utils.toString(createNativeAd(request.ad_id, request.layout_name, request.identifiers))
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

    private val testDeviceHash: String
        @AnyThread get() = ""

    @AnyThread
    fun addTestDevice(hash: String) {
        Thread.runOnMainThread(Runnable {
            AdSettings.addTestDevice(hash)
        })
    }

    @AnyThread
    fun clearTestDevices() {
        Thread.runOnMainThread(Runnable {
            AdSettings.clearTestDevices()
        })
    }

    @AnyThread
    fun getBannerAdSize(sizeId: Int): Point {
        return _bannerHelper.getSize(sizeId)
    }

    @AnyThread
    fun createBannerAd(adId: String, adSize: AdSize): Boolean {
        if (_bannerAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookBannerAd(_bridge, _context, _activity, adId, adSize)
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
        val ad = FacebookNativeAd(_bridge, _context, _activity, adId, layoutName, identifiers)
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
    private fun createInterstitialAd(adId: String): Boolean {
        if (_interstitialAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookInterstitialAd(_bridge, _context, adId)
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
    private fun createRewardedAd(adId: String): Boolean {
        if (_rewardedAds.containsKey(adId)) {
            return false
        }
        val ad = FacebookRewardedAd(_bridge, _context, adId)
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