package com.ee

import android.app.Activity
import android.app.Application
import android.graphics.Color

import android.util.Log

import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout

import androidx.annotation.AnyThread
import com.applovin.mediation.ads.MaxAdView
import com.applovin.mediation.ads.MaxInterstitialAd
import com.applovin.mediation.ads.MaxRewardedAd
import com.applovin.sdk.AppLovinSdk
import com.applovin.sdk.AppLovinSdkUtils
import com.appsflyer.adrevenue.AppsFlyerAdRevenue
import com.appsflyer.adrevenue.adnetworks.generic.MediationNetwork
import com.appsflyer.adrevenue.adnetworks.generic.Scheme

import com.appsflyer.api.PurchaseClient
import com.appsflyer.api.Store
import com.appsflyer.internal.models.InAppPurchaseValidationResult
import com.appsflyer.internal.models.SubscriptionValidationResult

import com.ee.applovinmax.R
import com.ee.internal.AppLovinMaxBannerAdListener

import com.ee.internal.AppLovinMaxInterstitialAdListener
import com.ee.internal.AppLovinMaxRewardedAdListener
import com.ee.internal.deserialize
import com.ee.internal.serialize
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.Serializable
import java.util.*
import kotlin.coroutines.resume


class AppLovinMaxBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {

    @Serializable
    private class InitializeRequest(
        val bannerAdId: String,
        val rewardedAdId: String,
        val interstitialAdId: String,
    )

    @Serializable
    private class CreateBannerAdRequest(
        val adId: String,
        val adSize: Int
    )

    @Serializable
    @Suppress("unused")
    private class GetBannerAdSizeResponse(
        val width: Int,
        val height: Int
    )

    companion object {
        private val kTag = AppLovinMaxBridge::class.java.name
        private const val kPrefix = "AppLovinMaxBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetVerboseLogging = "${kPrefix}SetVerboseLogging"
        private const val kSetMuted = "${kPrefix}SetMuted"

        private const val kCreateBannerAd = "${kPrefix}CreateBannerAd"
        private const val kLoadBannerAd = "${kPrefix}LoadBanner"
        private const val kGetBannerAdSize = "${kPrefix}GetBannerAdSize"
        private const val kSetBannerVisible = "${kPrefix}SetBannerVisible"
        private const val kSetBannerPosition = "${kPrefix}_setPosition_"
        private const val kSetBannerSize = "${kPrefix}_setSize_"
        private const val kDestroyAd = "${kPrefix}DestroyAd"

        private const val kHasInterstitialAd = "${kPrefix}HasInterstitialAd"
        private const val kLoadInterstitialAd = "${kPrefix}LoadInterstitialAd"
        private const val kShowInterstitialAd = "${kPrefix}ShowInterstitialAd"

        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kLoadRewardedAd = "${kPrefix}LoadRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
    }

    private var _initializing = false
    private var _initialized = false
    private var _sdk: AppLovinSdk? = null

    private var _bannerAdId: String? = null
    private var _banner: MaxAdView? = null
    private var _bannerListener: AppLovinMaxBannerAdListener? = null

    private var _interstitialAd: MaxInterstitialAd? = null
    private var _interstitialAdListener: AppLovinMaxInterstitialAdListener? = null
    private var _rewardedAd: MaxRewardedAd? = null
    private var _rewardedAdListener: AppLovinMaxRewardedAdListener? = null

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()

        // init appsflyer
        // af_ad_revenue
        val application = _activity!!.application
        val afRevenueBuilder = AppsFlyerAdRevenue.Builder(application)
        AppsFlyerAdRevenue.initialize(afRevenueBuilder.build())

        log("Init af_ad_revenue success")

        // af_purchase
        val builder = PurchaseClient.Builder(application, Store.GOOGLE)
            .logSubscriptions(true)
            .autoLogInApps(true)
            .setSandbox(false)
            .setInAppValidationResultListener(AppsFlyerIapResultListener())
            .setSubscriptionValidationResultListener(AppsFlyerSubscriptionResultListener())

        val afPurchaseClient = builder.build()
        afPurchaseClient.startObservingTransactions()

        log("Init af_purchase success")

        _logger.info("$kTag: constructor end")
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
            _sdk = null
            _interstitialAd = null
            _interstitialAdListener = null
            _rewardedAd = null
            _rewardedAdListener = null
        }
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            val request = deserialize<InitializeRequest>(message)
            Utils.toString(
                initialize(
                    request.bannerAdId,
                    request.rewardedAdId,
                    request.interstitialAdId
                )
            )
        }
        _bridge.registerHandler(kSetVerboseLogging) { message ->
            setVerboseLogging(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetMuted) { message ->
            setMuted(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kCreateBannerAd) { _ ->
            // do nothing
            Utils.toString(true)
        }
        _bridge.registerHandler(kLoadBannerAd) { _ ->
            // do nothing
            Utils.toString(showBanner())
        }
        _bridge.registerHandler(kGetBannerAdSize) { _ ->
            val response = GetBannerAdSizeResponse(0, 0)
            response.serialize()
        }
        _bridge.registerHandler(kSetBannerVisible) { message ->
            Utils.toString(setBannerVisible(message))
        }
        _bridge.registerHandler(kDestroyAd) { _ ->
            hideBanner()
            Utils.toString(true)
        }
        _bridge.registerHandler(kHasInterstitialAd) {
            Utils.toString(hasInterstitialAd)
        }
        _bridge.registerHandler(kLoadInterstitialAd) {
            loadInterstitialAd()
            ""
        }
        _bridge.registerHandler(kShowInterstitialAd) {
            showInterstitialAd()
            ""
        }
        _bridge.registerHandler(kHasRewardedAd) {
            Utils.toString(hasRewardedAd)
        }
        _bridge.registerHandler(kLoadRewardedAd) {
            loadRewardedAd()
            ""
        }
        _bridge.registerHandler(kShowRewardedAd) {
            showRewardedAd()
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetVerboseLogging)
        _bridge.deregisterHandler(kSetMuted)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)

        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kDestroyAd)
        _bridge.deregisterHandler(kSetBannerVisible)
        _bridge.deregisterHandler(kSetBannerPosition + _bannerAdId)
        _bridge.deregisterHandler(kSetBannerSize + _bannerAdId)
    }

    private fun checkInitialized() {
        if (!_initialized) {
            throw IllegalStateException("Please call initialize() first")
        }
    }

    @AnyThread
    suspend fun initialize(
        bannerAdId: String,
        rewardedAdId: String,
        interstitialAdId: String
    ): Boolean {
        return suspendCancellableCoroutine { cont ->
            Thread.runOnMainThread {
                if (_initializing) {
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (_initialized) {
                    cont.resume(true)
                    return@runOnMainThread
                }
                _initializing = true

                val sdk = AppLovinSdk.getInstance(_application)
                sdk.mediationProvider = "max"
                sdk.initializeSdk {
                    if (cont.isActive) {
                        // Enable Test Ads?
//                         sdk.showMediationDebugger()
                    } else {
                        return@initializeSdk
                    }
                    Thread.runOnMainThread {
                        _initializing = false
                        _initialized = true
                        initAds(bannerAdId, interstitialAdId, rewardedAdId)
                        cont.resume(true)
                    }
                }

                _sdk = sdk
            }
        }
    }

    @AnyThread
    fun initAds(bannerAdId: String, interstitialAdId: String, rewardedAdId: String) {
        Thread.runOnMainThread {
            // Banner
            val bannerListener = AppLovinMaxBannerAdListener(_bridge, _logger, bannerAdId)
            { d -> logAppsFlyerAdRevenue(d) }
            _bridge.registerHandler(kSetBannerPosition + bannerAdId) {
                // do nothing
                Utils.toString(true)
            }
            _bridge.registerHandler(kSetBannerSize + bannerAdId) {
                // do nothing
                Utils.toString(true)
            }
            createBanner(bannerAdId, bannerListener)

            // Interstitial
            val interstitialAd = MaxInterstitialAd(interstitialAdId, _activity)
            val interstitialAdListener = AppLovinMaxInterstitialAdListener(
                interstitialAdId, _bridge, _logger
            ) { d -> logAppsFlyerAdRevenue(d) }
            interstitialAd.setListener(interstitialAdListener)
            interstitialAd.setRevenueListener(interstitialAdListener)

            // Rewarded
            val rewardedAd = MaxRewardedAd.getInstance(rewardedAdId, _activity)
            val rewardedAdListener = AppLovinMaxRewardedAdListener(
                rewardedAdId, _bridge, _logger
            ) { d -> logAppsFlyerAdRevenue(d) }
            rewardedAd.setListener(rewardedAdListener)
            rewardedAd.setRevenueListener(rewardedAdListener)

            _bannerAdId = bannerAdId
            _bannerListener = bannerListener
            _interstitialAd = interstitialAd
            _interstitialAdListener = interstitialAdListener
            _rewardedAd = rewardedAd
            _rewardedAdListener = rewardedAdListener
        }
    }

    @AnyThread
    fun setVerboseLogging(enabled: Boolean) {
        Thread.runOnMainThread {
            checkInitialized()
            val sdk = _sdk ?: return@runOnMainThread
            sdk.settings.setVerboseLogging(enabled)
        }
    }

    @AnyThread
    fun setMuted(enabled: Boolean) {
        Thread.runOnMainThread {
            checkInitialized()
            val sdk = _sdk ?: return@runOnMainThread
            sdk.settings.isMuted = enabled
        }
    }

    private val hasInterstitialAd: Boolean
        @AnyThread get() = _interstitialAd?.isReady ?: false

    @AnyThread
    fun loadInterstitialAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _interstitialAd ?: return@runOnMainThread
            ad.loadAd()
        }
    }

    @AnyThread
    fun showInterstitialAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _interstitialAd ?: return@runOnMainThread
            ad.showAd()
        }
    }

    val hasRewardedAd: Boolean
        @AnyThread get() = _rewardedAd?.isReady ?: false

    @AnyThread
    fun loadRewardedAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _rewardedAd ?: return@runOnMainThread
            ad.loadAd()
        }
    }

    @AnyThread
    fun showRewardedAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _rewardedAd ?: return@runOnMainThread
            ad.showAd()
        }
    }

    private fun logAppsFlyerAdRevenue(revenueData: AdRevenueData) {
        val customParams: MutableMap<String, String> = HashMap()
        customParams[Scheme.AD_UNIT] = revenueData.adUnitId
        customParams[Scheme.AD_TYPE] = revenueData.adFormat
        customParams[Scheme.PLACEMENT] = "place"
        customParams[Scheme.ECPM_PAYLOAD] = "encrypt"

        AppsFlyerAdRevenue.logAdRevenue(
            revenueData.networkName,
            MediationNetwork.applovinmax,
            Currency.getInstance(Locale.US),
            revenueData.revenue,
            customParams
        )
    }

    private fun setBannerVisible(message: String): Boolean {
        val visible = Utils.toBoolean(message)
        return if (visible) {
            showBanner()
        } else {
            hideBanner()
        }
    }

    private fun createBanner(bannerAdId: String, bannerListener: AppLovinMaxBannerAdListener) {
        checkInitialized()
        if (_banner != null) {
            return
        }
        if (_activity == null) {
            return
        }
        val activity: Activity = _activity as Activity

        val banner = MaxAdView(bannerAdId, activity)
        banner.setListener(bannerListener)
        banner.setRevenueListener(bannerListener)
        _banner = banner

        val isTablet = AppLovinSdkUtils.isTablet(activity)
        val width = AppLovinSdkUtils.dpToPx(activity, if (isTablet) 728 else 320)
        val height = AppLovinSdkUtils.dpToPx(activity, if (isTablet) 90 else 50)
        banner.layoutParams = FrameLayout.LayoutParams(width, height, Gravity.BOTTOM or Gravity.CENTER_HORIZONTAL)
        banner.setBackgroundColor(Color.TRANSPARENT)
        val rootView = activity.findViewById<ViewGroup>(android.R.id.content)
        rootView?.addView(banner)

        banner.loadAd()
    }

    private fun showBanner(): Boolean {
        if (_banner == null) {
            return true;
        }
        val banner = _banner as MaxAdView
        Thread.runOnMainThread {
            banner.visibility = View.VISIBLE
            banner.startAutoRefresh()
        }

        return true
    }

    private fun hideBanner(): Boolean {
        if (_banner == null) {
            return true
        }
        val banner = _banner as MaxAdView
        Thread.runOnMainThread {
            banner.setExtraParameter( "allow_pause_auto_refresh_immediately", "true" )
            banner.stopAutoRefresh()
            banner.visibility = View.INVISIBLE
        }
        return true
    }

    private fun log(message: String) {
        Log.d("AppsFlyer", message)
    }
}

class AppsFlyerIapResultListener : PurchaseClient.InAppPurchaseValidationResultListener {
    override fun onResponse(result: Map<String, InAppPurchaseValidationResult>?) {
        result?.forEach { (k: String, v: InAppPurchaseValidationResult?) ->
            if (v.success) {
                val productPurchase = v.productPurchase
                log("Validation success: $k $productPurchase")
            } else {
                val failureData = v.failureData
                log("Validation fail: $k $failureData")
            }
        }
    }

    override fun onFailure(result: String, error: Throwable?) {
        log("Validation fail: $result, $error")
    }

    private fun log(message: String) {
        Log.d("AppsFlyer", message)
    }
}

class AppsFlyerSubscriptionResultListener :
    PurchaseClient.SubscriptionPurchaseValidationResultListener {
    override fun onResponse(result: MutableMap<String, out SubscriptionValidationResult>?) {
        result?.forEach { (k: String, v: SubscriptionValidationResult?) ->
            if (v.success) {
                val productPurchase = v.subscriptionPurchase
                log("Validation success: $k $productPurchase")
            } else {
                val failureData = v.failureData
                log("Validation fail: $k $failureData")
            }
        }
    }

    override fun onFailure(result: String, error: Throwable?) {
        log("Validation fail: $result, $error")
    }

    private fun log(message: String) {
        Log.d("AppsFlyer", message)
    }
}