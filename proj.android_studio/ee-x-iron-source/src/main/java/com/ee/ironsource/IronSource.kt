package com.ee.ironsource

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
import com.ironsource.mediationsdk.logger.IronSourceError
import com.ironsource.mediationsdk.model.Placement
import com.ironsource.mediationsdk.sdk.InterstitialListener
import com.ironsource.mediationsdk.sdk.RewardedVideoListener
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
class IronSource(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?)
    : IPlugin
    , RewardedVideoListener
    , InterstitialListener {
    companion object {
        private val _logger = Logger(IronSource::class.java.name)

        private const val kPrefix = "IronSource"
        private const val k__initialize = "${kPrefix}_initialize"
        private const val k__loadInterstitialAd = "${kPrefix}_loadInterstitialAd"
        private const val k__hasInterstitialAd = "${kPrefix}_hasInterstitialAd"
        private const val k__showInterstitialAd = "${kPrefix}_showInterstitialAd"
        private const val k__hasRewardedAd = "${kPrefix}_hasRewardedAd"
        private const val k__showRewardedAd = "${kPrefix}_showRewardedAd"
        private const val k__onInterstitialAdLoaded = "${kPrefix}_onInterstitialAdLoaded"
        private const val k__onInterstitialAdFailedToLoad = "${kPrefix}_onInterstitialAdFailedToLoad"
        private const val k__onInterstitialAdFailedToShow = "${kPrefix}_onInterstitialAdFailedToShow"
        private const val k__onInterstitialAdClicked = "${kPrefix}_onInterstitialAdClicked"
        private const val k__onInterstitialAdClosed = "${kPrefix}_onInterstitialAdClosed"
        private const val k__onRewardedAdLoaded = "${kPrefix}_onRewardedAdLoaded"
        private const val k__onRewardedAdFailedToShow = "${kPrefix}_onRewardedAdFailedToShow"
        private const val k__onRewardedAdClicked = "${kPrefix}_onRewardedAdClicked"
        private const val k__onRewardedAdClosed = "${kPrefix}_onRewardedAdClosed"
    }

    private var _initialized = false
    private val _isInterstitialAdLoaded = AtomicBoolean(false)
    private val _isRewardedAdLoaded = AtomicBoolean(false)
    private var _rewarded = false

    init {
        _logger.debug("constructor begin.")
        registerHandlers()
        _logger.debug("constructor end.")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {
        Thread.checkMainThread()
        com.ironsource.mediationsdk.IronSource.onResume(_activity)
    }

    override fun onPause() {
        Thread.checkMainThread()
        com.ironsource.mediationsdk.IronSource.onPause(_activity)
    }

    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            if (!_initialized) {
                return@Runnable
            }
            com.ironsource.mediationsdk.IronSource.setInterstitialListener(null)
            com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(null)
        })
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__initialize) { message ->
            initialize(message)
            ""
        }
        _bridge.registerHandler(k__loadInterstitialAd) {
            loadInterstitialAd()
            ""
        }
        _bridge.registerHandler(k__hasInterstitialAd) {
            Utils.toString(hasInterstitialAd)
        }
        _bridge.registerHandler(k__showInterstitialAd) { message ->
            showInterstitialAd(message)
            ""
        }
        _bridge.registerHandler(k__hasRewardedAd) {
            Utils.toString(hasRewardedAd)
        }
        _bridge.registerHandler(k__showRewardedAd) { message ->
            showRewardedAd(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize)
        _bridge.deregisterHandler(k__loadInterstitialAd)
        _bridge.deregisterHandler(k__hasInterstitialAd)
        _bridge.deregisterHandler(k__showInterstitialAd)
        _bridge.deregisterHandler(k__hasRewardedAd)
        _bridge.deregisterHandler(k__showRewardedAd)
    }

    @AnyThread
    fun initialize(gameId: String) {
        Thread.runOnMainThread(Runnable {
            if (_initialized) {
                return@Runnable
            }
            com.ironsource.mediationsdk.IronSource.init(_activity, gameId,
                com.ironsource.mediationsdk.IronSource.AD_UNIT.REWARDED_VIDEO,
                com.ironsource.mediationsdk.IronSource.AD_UNIT.INTERSTITIAL)
            com.ironsource.mediationsdk.IronSource.shouldTrackNetworkState(_activity, true)
            com.ironsource.mediationsdk.IronSource.setInterstitialListener(this)
            com.ironsource.mediationsdk.IronSource.setRewardedVideoListener(this)
            _initialized = true
        })
    }

    @AnyThread
    fun loadInterstitialAd() {
        Thread.runOnMainThread(Runnable {
            _logger.debug(this::loadInterstitialAd.name)
            com.ironsource.mediationsdk.IronSource.loadInterstitial()
        })
    }

    val hasInterstitialAd: Boolean
        @AnyThread get() = _isInterstitialAdLoaded.get()

    @AnyThread
    fun showInterstitialAd(adId: String) {
        Thread.runOnMainThread(Runnable {
            _logger.debug("${this::showInterstitialAd.name}: $adId")
            com.ironsource.mediationsdk.IronSource.showInterstitial(adId)
        })
    }

    val hasRewardedAd: Boolean
        @AnyThread get() = _isRewardedAdLoaded.get()

    @AnyThread
    fun showRewardedAd(adId: String) {
        Thread.runOnMainThread(Runnable {
            _logger.debug("${this::showRewardedAd.name}: $adId")
            _rewarded = false
            com.ironsource.mediationsdk.IronSource.showRewardedVideo(adId)
        })
    }

    private fun handleRewardedAdResult() {
        _isRewardedAdLoaded.set(false)
        _bridge.callCpp(k__onRewardedAdClosed, Utils.toString(_rewarded))
    }

    override fun onInterstitialAdReady() {
        _logger.debug(this::onInterstitialAdReady.name)
        _isInterstitialAdLoaded.set(true)
        _bridge.callCpp(k__onInterstitialAdLoaded)
    }

    override fun onInterstitialAdLoadFailed(ironSourceError: IronSourceError) {
        _logger.debug("${this::onInterstitialAdLoadFailed.name}: ${ironSourceError.errorMessage}")
        _bridge.callCpp(k__onInterstitialAdFailedToLoad, ironSourceError.errorMessage)
    }

    override fun onInterstitialAdShowSucceeded() {
        _logger.debug(this::onInterstitialAdShowSucceeded.name)
    }

    override fun onInterstitialAdShowFailed(ironSourceError: IronSourceError) {
        _logger.debug("${this::onInterstitialAdShowFailed.name}: ${ironSourceError.errorMessage}")
        _bridge.callCpp(k__onInterstitialAdFailedToShow, ironSourceError.errorMessage)
    }

    override fun onInterstitialAdOpened() {
        _logger.debug(this::onInterstitialAdOpened.name)
    }

    override fun onInterstitialAdClicked() {
        _logger.debug(this::onInterstitialAdClicked.name)
        _bridge.callCpp(k__onInterstitialAdClicked)
    }

    override fun onInterstitialAdClosed() {
        _logger.debug(this::onInterstitialAdClosed.name)
        _isInterstitialAdLoaded.set(false)
        _bridge.callCpp(k__onInterstitialAdClosed)
    }

    override fun onRewardedVideoAvailabilityChanged(available: Boolean) {
        _logger.info("${this::onRewardedVideoAvailabilityChanged.name}: $available")
        if (available) {
            _isRewardedAdLoaded.set(true)
            _bridge.callCpp(k__onRewardedAdLoaded)
        }
    }

    override fun onRewardedVideoAdShowFailed(ironSourceError: IronSourceError) {
        _logger.debug("${this::onRewardedVideoAdShowFailed.name}: ${ironSourceError.errorMessage}")
        _bridge.callCpp(k__onRewardedAdFailedToShow, ironSourceError.errorMessage)
    }

    override fun onRewardedVideoAdOpened() {
        _logger.debug(this::onRewardedVideoAdOpened.name)
    }

    override fun onRewardedVideoAdStarted() {
        _logger.debug(this::onRewardedVideoAdStarted.name)
    }

    override fun onRewardedVideoAdClicked(placement: Placement) {
        _logger.debug("${this::onRewardedVideoAdClicked.name}: ${placement.placementName}")
        _bridge.callCpp(k__onRewardedAdClicked)
    }

    override fun onRewardedVideoAdEnded() {
        _logger.debug(this::onRewardedVideoAdEnded.name)
    }

    override fun onRewardedVideoAdRewarded(placement: Placement) {
        _logger.debug("${this::onRewardedVideoAdRewarded.name}: ${placement.placementName}")
        _rewarded = true
    }

    override fun onRewardedVideoAdClosed() {
        _logger.debug(this::onRewardedVideoAdClosed.name)
        if (_rewarded) {
            handleRewardedAdResult()
        } else {
            // Note: The onRewardedVideoAdRewarded and onRewardedVideoAdClosed events are
            // asynchronous.
            Thread.runOnMainThreadDelayed(1.0f, Runnable {
                handleRewardedAdResult()
            })
        }
    }
}