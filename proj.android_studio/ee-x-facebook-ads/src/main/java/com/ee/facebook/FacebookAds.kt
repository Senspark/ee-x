package com.ee.facebook

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.Point
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
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
class FacebookAds(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private const val kPrefix = "FacebookAds"
        private const val k__getTestDeviceHash = "${kPrefix}_getTestDeviceHash"
        private const val k__addTestDevice = "${kPrefix}_addTestDevice"
        private const val k__clearTestDevices = "${kPrefix}_clearTestDevices"
        private const val k__getBannerAdSize = "${kPrefix}_getBannerAdSize"
        private const val k__createBannerAd = "${kPrefix}_createBannerAd"
        private const val k__destroyBannerAd = "${kPrefix}_destroyBannerAd"
        private const val k__createNativeAd = "${kPrefix}_createNativeAd"
        private const val k__destroyNativeAd = "${kPrefix}_destroyNativeAd"
        private const val k__createInterstitialAd = "${kPrefix}_createInterstitialAd"
        private const val k__destroyInterstitialAd = "${kPrefix}_destroyInterstitialAd"
        private const val k__createRewardedAd = "${kPrefix}_createRewardedAd"
        private const val k__destroyRewardedAd = "${kPrefix}_destroyRewardedAd"
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

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        return false
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
        _bridge.registerHandler(k__getTestDeviceHash) {
            testDeviceHash
        }
        _bridge.registerHandler(k__addTestDevice) { message ->
            addTestDevice(message)
            ""
        }
        _bridge.registerHandler(k__clearTestDevices) {
            clearTestDevices()
            ""
        }
        _bridge.registerHandler(k__getBannerAdSize) { message ->
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
        _bridge.registerHandler(k__createBannerAd) { message ->
            @Serializable
            class Request(
                val ad_id: String,
                val ad_size: Int
            )

            val request = deserialize<Request>(message)
            val adSize = _bannerHelper.getAdSize(request.ad_size)
            Utils.toString(createBannerAd(request.ad_id, adSize))
        }
        _bridge.registerHandler(k__destroyBannerAd) { message ->
            Utils.toString(destroyBannerAd(message))
        }
        _bridge.registerHandler(k__createNativeAd) { message ->
            @Serializable
            class Request(
                val ad_id: String,
                val layout_name: String,
                val identifiers: Map<String, String>
            )

            val request = deserialize<Request>(message)
            Utils.toString(createNativeAd(request.ad_id, request.layout_name, request.identifiers))
        }
        _bridge.registerHandler(k__destroyNativeAd) { message ->
            Utils.toString(destroyNativeAd(message))
        }
        _bridge.registerHandler(k__createInterstitialAd) { message ->
            Utils.toString(createInterstitialAd(message))
        }
        _bridge.registerHandler(k__destroyInterstitialAd) { message ->
            Utils.toString(destroyInterstitialAd(message))
        }
        _bridge.registerHandler(k__createRewardedAd) { message ->
            Utils.toString(createRewardedAd(message))
        }
        _bridge.registerHandler(k__destroyRewardedAd) { message ->
            Utils.toString(destroyRewardedAd(message))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__getTestDeviceHash)
        _bridge.deregisterHandler(k__addTestDevice)
        _bridge.deregisterHandler(k__clearTestDevices)
        _bridge.deregisterHandler(k__getBannerAdSize)
        _bridge.deregisterHandler(k__createBannerAd)
        _bridge.deregisterHandler(k__destroyBannerAd)
        _bridge.deregisterHandler(k__createNativeAd)
        _bridge.deregisterHandler(k__destroyNativeAd)
        _bridge.deregisterHandler(k__createInterstitialAd)
        _bridge.deregisterHandler(k__destroyInterstitialAd)
        _bridge.deregisterHandler(k__createRewardedAd)
        _bridge.deregisterHandler(k__destroyRewardedAd)
    }

    val testDeviceHash: String
        @AnyThread get() = ""

    @UiThread
    fun addTestDevice(hash: String) {
        AdSettings.addTestDevice(hash)
    }

    @UiThread
    fun clearTestDevices() {
        AdSettings.clearTestDevices()
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