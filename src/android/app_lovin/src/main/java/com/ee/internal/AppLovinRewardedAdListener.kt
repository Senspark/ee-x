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
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinRewardedAdListener(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger)
    : AppLovinAdLoadListener, AppLovinAdDisplayListener, AppLovinAdClickListener, AppLovinAdRewardListener, AppLovinAdVideoPlaybackListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )

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
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::adReceived.name}")
            _isLoaded.set(true)
            _bridge.callCpp(kOnRewardedAdLoaded)
        }
    }

    override fun failedToReceiveAd(errorCode: Int) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::failedToReceiveAd.name}: code $errorCode")
            _bridge.callCpp(kOnRewardedAdFailedToLoad, ErrorResponse(errorCode, "").serialize())
        }
    }

    override fun adDisplayed(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::adDisplayed.name}")
            _rewarded = false
        }
    }

    override fun videoPlaybackBegan(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::videoPlaybackBegan.name}")
        }
    }

    override fun adClicked(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::adClicked.name}")
            _bridge.callCpp(kOnRewardedAdClicked)
        }
    }

    override fun videoPlaybackEnded(ad: AppLovinAd, percentViewed: Double, fullyWatched: Boolean) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::videoPlaybackEnded.name}")
        }
    }

    override fun userRewardVerified(ad: AppLovinAd, response: Map<String, String>) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::userRewardVerified.name}: $response")
            _rewarded = true
        }
    }

    override fun userRewardRejected(ad: AppLovinAd, response: Map<String, String>) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::userRewardRejected.name}: $response")
        }
    }

    override fun userOverQuota(ad: AppLovinAd, response: Map<String, String>) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::userOverQuota.name}: $response")
        }
    }

    override fun validationRequestFailed(ad: AppLovinAd, responseCode: Int) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::validationRequestFailed.name}: code = $responseCode")
        }
    }

    override fun adHidden(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::adHidden.name}")
            _isLoaded.set(false)
            _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
        }
    }
}