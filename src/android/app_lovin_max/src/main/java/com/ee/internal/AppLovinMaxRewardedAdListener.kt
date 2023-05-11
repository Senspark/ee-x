package com.ee.internal

import com.applovin.mediation.*
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinMaxRewardedAdListener(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger)
    : MaxRewardedAdListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )
    companion object {
        private val kTag = AppLovinMaxRewardedAdListener::class.java.name
        private const val kPrefix = "AppLovinMaxBridge"
        private const val kOnRewardedAdLoaded = "${kPrefix}OnRewardedAdLoaded"
        private const val kOnRewardedAdFailedToLoad = "${kPrefix}OnRewardedAdFailedToLoad"
        private const val kOnRewardedAdClicked = "${kPrefix}OnRewardedAdClicked"
        private const val kOnRewardedAdClosed = "${kPrefix}OnRewardedAdClosed"
    }

    private val _isLoaded = AtomicBoolean(false)
    private var _rewarded = false

    val isLoaded: Boolean
        get() = _isLoaded.get()

    override fun onAdLoaded(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name}")
            _isLoaded.set(true)
            _bridge.callCpp(kOnRewardedAdLoaded)
        }
    }

    override fun onAdDisplayed(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdDisplayed.name}")
            _rewarded = false
        }
    }

    override fun onAdHidden(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdHidden.name}")
            _isLoaded.set(false)
            _bridge.callCpp(kOnRewardedAdClosed, Utils.toString(_rewarded))
        }
    }

    override fun onAdClicked(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdClicked.name}")
            _bridge.callCpp(kOnRewardedAdClicked)
        }
    }

    override fun onAdLoadFailed(p0: String?, p1: MaxError?) {
        Thread.runOnMainThread {
            val errCode = p1?.code ?: 0;
            val errMsg = p1?.message ?: "";
            _logger.debug("$kTag: ${this::onAdLoadFailed.name}: code $errCode")
            _bridge.callCpp(kOnRewardedAdFailedToLoad, ErrorResponse(errCode, errMsg).serialize())
        }
    }

    override fun onAdDisplayFailed(p0: MaxAd?, p1: MaxError?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdDisplayFailed.name}")
        }
    }

    override fun onUserRewarded(p0: MaxAd?, p1: MaxReward?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onUserRewarded.name}: ${p1?.amount} ${p1?.label}")
            _rewarded = true
        }
    }

    override fun onRewardedVideoStarted(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoStarted.name}")
        }
    }

    override fun onRewardedVideoCompleted(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoCompleted.name}")
        }
    }
}