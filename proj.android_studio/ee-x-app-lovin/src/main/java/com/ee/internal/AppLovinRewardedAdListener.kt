package com.ee.internal

import com.applovin.sdk.AppLovinAd
import com.applovin.sdk.AppLovinAdClickListener
import com.applovin.sdk.AppLovinAdDisplayListener
import com.applovin.sdk.AppLovinAdLoadListener
import com.applovin.sdk.AppLovinAdRewardListener
import com.applovin.sdk.AppLovinAdVideoPlaybackListener
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinRewardedAdListener(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger)
    : AppLovinAdLoadListener, AppLovinAdDisplayListener, AppLovinAdClickListener, AppLovinAdRewardListener, AppLovinAdVideoPlaybackListener {
    companion object {
        private val kTag = AppLovinRewardedAdListener::class.java.name
        private const val kPrefix = "AppLovinBridge"
        private const val kOnRewardedAdLoaded = "${kPrefix}OnRewardedAdLoaded"
        private const val kOnRewardedAdFailedToLoad = "${kPrefix}OnRewardedAdFailedToLoad"
        private const val kOnRewardedAdClicked = "${kPrefix}OnRewardedAdClicked"
        private const val kOnRewardedAdClosed = "${kPrefix}OnRewardedAdClosed"
    }

    private val _isLoaded = AtomicBoolean(false)
    private var _rewarded = false

    val isLoaded: Boolean
        get() = _isLoaded.get()

    override fun adReceived(ad: AppLovinAd) {
        _logger.debug("$kTag: ${this::adReceived.name}")
        _isLoaded.set(true)
        _bridge.callCpp(kOnRewardedAdLoaded)
    }

    override fun failedToReceiveAd(errorCode: Int) {
        _logger.debug("$kTag: ${this::failedToReceiveAd.name}: code $errorCode")
        _bridge.callCpp(kOnRewardedAdFailedToLoad, errorCode.toString())
    }

    override fun userDeclinedToViewAd(ad: AppLovinAd) {
        _logger.debug("$kTag: ${this::userDeclinedToViewAd.name}")
        Thread.checkMainThread()
    }

    override fun adDisplayed(ad: AppLovinAd) {
        _logger.debug("$kTag: ${this::adDisplayed.name}")
        Thread.checkMainThread()
        _rewarded = false
    }

    override fun videoPlaybackBegan(ad: AppLovinAd) {
        _logger.debug("$kTag: ${this::videoPlaybackBegan.name}")
    }

    override fun adClicked(ad: AppLovinAd) {
        _logger.debug("$kTag: ${this::adClicked.name}")
        Thread.checkMainThread()
        _bridge.callCpp(kOnRewardedAdClicked)
    }

    override fun videoPlaybackEnded(ad: AppLovinAd, percentViewed: Double, fullyWatched: Boolean) {
        _logger.debug("$kTag: ${this::videoPlaybackEnded.name}")
    }

    override fun userRewardVerified(ad: AppLovinAd, response: Map<String, String>) {
        _logger.info("$kTag: ${this::userRewardVerified.name}: $response")
        Thread.checkMainThread()
        _rewarded = true
    }

    override fun userRewardRejected(ad: AppLovinAd, response: Map<String, String>) {
        _logger.info("$kTag: ${this::userRewardRejected.name}: $response")
        Thread.checkMainThread()
    }

    override fun userOverQuota(ad: AppLovinAd, response: Map<String, String>) {
        _logger.info("$kTag: ${this::userOverQuota.name}: $response")
        Thread.checkMainThread()
    }

    override fun validationRequestFailed(ad: AppLovinAd, responseCode: Int) {
        _logger.info("$kTag: ${this::validationRequestFailed.name}: code = $responseCode")
        Thread.checkMainThread()
    }

    override fun adHidden(ad: AppLovinAd) {
        _logger.info("$kTag: ${this::adHidden.name}")
        Thread.checkMainThread()
        _isLoaded.set(false)
        _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
    }
}