package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.IronSourceBannerAd
import com.ee.internal.IronSourceBannerHelper
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.ironsource.mediationsdk.ISBannerSize
import com.ironsource.mediationsdk.IronSource
import com.ironsource.mediationsdk.logger.IronSourceError
import com.ironsource.mediationsdk.model.Placement
import com.ironsource.mediationsdk.sdk.InterstitialListener
import com.ironsource.mediationsdk.sdk.RewardedVideoListener
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.util.concurrent.ConcurrentHashMap
import java.util.concurrent.atomic.AtomicBoolean
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@InternalSerializationApi
class IronSourceBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?)
    : IPlugin, RewardedVideoListener, InterstitialListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )

    companion object {
        private val kTag = IronSourceBridge::class.java.name
        private const val kPrefix = "IronSourceBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kGetBannerAdSize = "${kPrefix}GetBannerAdSize"
        private const val kCreateBannerAd = "${kPrefix}CreateBannerAd"
        private const val kDestroyAd = "${kPrefix}DestroyAd"
        private const val kHasInterstitialAd = "${kPrefix}HasInterstitialAd"
        private const val kLoadInterstitialAd = "${kPrefix}LoadInterstitialAd"
        private const val kShowInterstitialAd = "${kPrefix}ShowInterstitialAd"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
        private const val kOnInterstitialAdLoaded = "${kPrefix}OnInterstitialAdLoaded"
        private const val kOnInterstitialAdFailedToLoad = "${kPrefix}OnInterstitialAdFailedToLoad"
        private const val kOnInterstitialAdFailedToShow = "${kPrefix}OnInterstitialAdFailedToShow"
        private const val kOnInterstitialAdClicked = "${kPrefix}OnInterstitialAdClicked"
        private const val kOnInterstitialAdClosed = "${kPrefix}OnInterstitialAdClosed"
        private const val kOnRewardedAdLoaded = "${kPrefix}OnRewardedAdLoaded"
        private const val kOnRewardedAdFailedToShow = "${kPrefix}OnRewardedAdFailedToShow"
        private const val kOnRewardedAdClicked = "${kPrefix}OnRewardedAdClicked"
        private const val kOnRewardedAdClosed = "${kPrefix}OnRewardedAdClosed"
    }

    private var _initialized = false
    private val _bannerHelper = IronSourceBannerHelper()
    private val _ads: MutableMap<String, IAd> = ConcurrentHashMap()
    private val _isInterstitialAdLoaded = AtomicBoolean(false)
    private val _isRewardedAdLoaded = AtomicBoolean(false)
    private var _rewarded = false

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
        Thread.checkMainThread()
        IronSource.onResume(_activity)
        for (ad in _ads.values) {
            ad.onResume()
        }
    }

    override fun onPause() {
        Thread.checkMainThread()
        IronSource.onPause(_activity)
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
        IronSource.removeInterstitialListener()
        IronSource.removeRewardedVideoListener()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            Utils.toString(initialize(message))
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
        _bridge.registerHandler(kLoadInterstitialAd) {
            loadInterstitialAd()
            ""
        }
        _bridge.registerHandler(kHasInterstitialAd) {
            Utils.toString(hasInterstitialAd)
        }
        _bridge.registerHandler(kShowInterstitialAd) { message ->
            showInterstitialAd(message)
            ""
        }
        _bridge.registerHandler(kHasRewardedAd) {
            Utils.toString(hasRewardedAd)
        }
        _bridge.registerHandler(kShowRewardedAd) { message ->
            showRewardedAd(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kGetBannerAdSize)
        _bridge.deregisterHandler(kCreateBannerAd)
        _bridge.deregisterHandler(kDestroyAd)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    private fun checkInitialized() {
        if (!_initialized) {
            throw IllegalStateException("Please call initialize() first")
        }
    }

    @AnyThread
    suspend fun initialize(appKey: String): Boolean {
        return suspendCoroutine { cont ->
            Thread.runOnMainThread {
                if (_initialized) {
                    _logger.info("$kTag: initialize: initialized")
                    cont.resume(true)
                    return@runOnMainThread
                }
                IronSource.init(_activity, appKey, IronSource.AD_UNIT.REWARDED_VIDEO, IronSource.AD_UNIT.INTERSTITIAL, IronSource.AD_UNIT.BANNER)
                IronSource.shouldTrackNetworkState(_activity, true)
                IronSource.setInterstitialListener(this)
                IronSource.setRewardedVideoListener(this)
                IronSource.setUserId(IronSource.getAdvertiserId(_application))
                val banner = IronSource.createBanner(_activity, ISBannerSize.BANNER)
                banner.size
                _initialized = true
                _logger.info("$kTag: initialize: done")
                cont.resume(true)
            }
        }
    }

    @AnyThread
    fun createBannerAd(adId: String, adSize: ISBannerSize): Boolean {
        return createAd(adId) {
            IronSourceBannerAd(_bridge, _logger, _activity, adId, adSize, _bannerHelper)
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

    private val hasInterstitialAd: Boolean
        @AnyThread get() = _isInterstitialAdLoaded.get()

    @AnyThread
    fun loadInterstitialAd() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::loadInterstitialAd.name}")
            checkInitialized()
            IronSource.loadInterstitial()
        }
    }

    @AnyThread
    fun showInterstitialAd(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::showInterstitialAd.name}: id = $adId")
            checkInitialized()
            IronSource.showInterstitial(adId)
        }
    }

    val hasRewardedAd: Boolean
        @AnyThread get() = _isRewardedAdLoaded.get()

    @AnyThread
    fun showRewardedAd(adId: String) {
        Thread.runOnMainThread {
            _logger.debug("$kTag:${this::showRewardedAd.name}: id = $adId")
            checkInitialized()
            _rewarded = false
            IronSource.showRewardedVideo(adId)
        }
    }

    private fun handleRewardedAdResult() {
        _isRewardedAdLoaded.set(false)
        _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
    }

    override fun onInterstitialAdReady() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialAdReady.name}")
            _isInterstitialAdLoaded.set(true)
            _bridge.callCpp(kOnInterstitialAdLoaded)
        }
    }

    override fun onInterstitialAdLoadFailed(error: IronSourceError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialAdLoadFailed.name}: ${error.errorMessage}")
            _bridge.callCpp(kOnInterstitialAdFailedToLoad, ErrorResponse(error.errorCode, error.errorMessage).serialize())
        }
    }

    override fun onInterstitialAdShowSucceeded() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialAdShowSucceeded.name}")
        }
    }

    override fun onInterstitialAdShowFailed(error: IronSourceError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag:${this::onInterstitialAdShowFailed.name}: ${error.errorMessage}")
            _isInterstitialAdLoaded.set(false)
            _bridge.callCpp(kOnInterstitialAdFailedToShow, ErrorResponse(error.errorCode, error.errorMessage).serialize())
        }
    }

    override fun onInterstitialAdOpened() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialAdOpened.name}")
        }
    }

    override fun onInterstitialAdClicked() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialAdClicked.name}")
            _bridge.callCpp(kOnInterstitialAdClicked)
        }
    }

    override fun onInterstitialAdClosed() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialAdClosed.name}")
            _isInterstitialAdLoaded.set(false)
            _bridge.callCpp(kOnInterstitialAdClosed)
        }
    }

    override fun onRewardedVideoAvailabilityChanged(available: Boolean) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onRewardedVideoAvailabilityChanged.name}: $available")
            if (available) {
                _isRewardedAdLoaded.set(true)
                _bridge.callCpp(kOnRewardedAdLoaded)
            }
        }
    }

    override fun onRewardedVideoAdShowFailed(error: IronSourceError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdShowFailed.name}: ${error.errorMessage}")
            _isRewardedAdLoaded.set(false)
            _bridge.callCpp(kOnRewardedAdFailedToShow, ErrorResponse(error.errorCode, error.errorMessage).serialize())
        }
    }

    override fun onRewardedVideoAdOpened() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdOpened.name}")
        }
    }

    override fun onRewardedVideoAdStarted() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdStarted.name}")
        }
    }

    override fun onRewardedVideoAdClicked(placement: Placement?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdClicked.name}: ${placement?.placementName ?: ""}")
            _bridge.callCpp(kOnRewardedAdClicked)
        }
    }

    override fun onRewardedVideoAdEnded() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdEnded.name}")
        }
    }

    override fun onRewardedVideoAdRewarded(placement: Placement?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdRewarded.name}: ${placement?.placementName ?: ""}")
            _rewarded = true
        }
    }

    override fun onRewardedVideoAdClosed() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoAdClosed.name}")
            if (_rewarded) {
                handleRewardedAdResult()
            } else {
                // Note: The onRewardedVideoAdRewarded and onRewardedVideoAdClosed events are
                // asynchronous.
                Thread.runOnMainThreadDelayed(1.0f) {
                    handleRewardedAdResult()
                }
            }
        }
    }
}