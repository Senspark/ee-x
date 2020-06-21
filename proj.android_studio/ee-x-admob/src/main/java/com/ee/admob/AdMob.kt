package com.ee.admob

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.internal.NativeThread
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.internal.serialize
import com.ee.core.registerHandler
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.AdSize
import com.google.android.gms.ads.MobileAds
import com.google.android.gms.ads.RequestConfiguration
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 10/13/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
private class AdMob(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private const val kPrefix = "AdMob"
        private const val k__initialize = "${kPrefix}_initialize"
        private const val k__getEmulatorTestDeviceHash = "${kPrefix}_getEmulatorTestDeviceHash"
        private const val k__addTestDevice = "${kPrefix}_addTestDevice"
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

    private val _bannerHelper = AdMobBannerHelper(_context)
    private val _testDevices: MutableList<String> = ArrayList()
    private val _bannerAds: MutableMap<String, AdMobBannerAd> = HashMap()
    private val _nativeAds: MutableMap<String, AdMobNativeAd> = HashMap()
    private val _interstitialAds: MutableMap<String, AdMobInterstitialAd> = HashMap()
    private val _rewardedAds: MutableMap<String, AdMobRewardedAd> = HashMap()

    init {
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
        _bridge.registerHandler(k__initialize) {
            initialize()
            ""
        }
        _bridge.registerHandler(k__getEmulatorTestDeviceHash) {
            emulatorTestDeviceHash
        }
        _bridge.registerHandler(k__addTestDevice) { message ->
            addTestDevice(message)
            ""
        }
        _bridge.registerHandler(k__getBannerAdSize) { message ->
            @Serializable
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
                val layoutName: String,
                val identifiers: Map<String, String>
            )

            val request = deserialize<Request>(message)
            Utils.toString(createNativeAd(request.ad_id, request.layoutName, request.identifiers))
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

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize)
        _bridge.deregisterHandler(k__getEmulatorTestDeviceHash)
        _bridge.deregisterHandler(k__addTestDevice)
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

    fun initialize() {
        Thread.checkMainThread()
        MobileAds.initialize(_context)
    }

    val emulatorTestDeviceHash: String
        @AnyThread get() = AdRequest.DEVICE_ID_EMULATOR

    @NativeThread
    fun addTestDevice(hash: String) {
        Thread.checkMainThread()
        _testDevices.add(hash)
        val configuration = RequestConfiguration.Builder()
            .setTestDeviceIds(_testDevices)
            .build()
        MobileAds.setRequestConfiguration(configuration)
    }

    @NativeThread
    fun getBannerAdSize(sizeId: Int): Point {
        return _bannerHelper.getSize(sizeId)
    }

    @NativeThread
    fun createBannerAd(adId: String, size: AdSize): Boolean {
        if (_bannerAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobBannerAd(_bridge, _context, _activity, adId, size)
        _bannerAds[adId] = ad
        return true
    }

    @NativeThread
    fun destroyBannerAd(adId: String): Boolean {
        val ad = _bannerAds[adId] ?: return false
        ad.destroy()
        _bannerAds.remove(adId)
        return true
    }

    @NativeThread
    fun createNativeAd(adId: String, layoutName: String,
                       identifiers: Map<String, String>): Boolean {
        if (_nativeAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobNativeAd(_bridge, _context, _activity, adId, layoutName, identifiers)
        _nativeAds[adId] = ad
        return true
    }

    @NativeThread
    fun destroyNativeAd(adId: String): Boolean {
        val ad = _nativeAds[adId] ?: return false
        ad.destroy()
        _nativeAds.remove(adId)
        return true
    }

    @NativeThread
    fun createInterstitialAd(adId: String): Boolean {
        if (_interstitialAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobInterstitialAd(_bridge, _context, adId)
        _interstitialAds[adId] = ad
        return true
    }

    @NativeThread
    fun destroyInterstitialAd(adId: String): Boolean {
        val ad = _interstitialAds[adId] ?: return false
        ad.destroy()
        _interstitialAds.remove(adId)
        return true
    }

    @NativeThread
    fun createRewardedAd(adId: String): Boolean {
        if (_rewardedAds.containsKey(adId)) {
            return false
        }
        val ad = AdMobRewardedAd(_bridge, _context, _activity, adId)
        _rewardedAds[adId] = ad
        return true
    }

    @NativeThread
    fun destroyRewardedAd(adId: String): Boolean {
        val ad = _rewardedAds[adId] ?: return false
        ad.destroy()
        _rewardedAds.remove(adId)
        return true
    }
}