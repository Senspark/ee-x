package com.ee.internal

import androidx.annotation.AnyThread
import com.applovin.mediation.MaxAd
import com.applovin.mediation.MaxAdListener
import com.applovin.mediation.MaxError
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinMaxInterstitialAdListener(
    private val _id: String,
    private val _bridge: IMessageBridge,
    private val _logger: ILogger)
    : MaxAdListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )

    companion object {
        private val kTag = AppLovinMaxInterstitialAdListener::class.java.name
        private const val kPrefix = "AppLovinMaxBridge"
        private const val kOnInterstitialAdLoaded = "${kPrefix}OnInterstitialAdLoaded"
        private const val kOnInterstitialAdFailedToLoad = "${kPrefix}OnInterstitialAdFailedToLoad"
        private const val kOnInterstitialAdClicked = "${kPrefix}OnInterstitialAdClicked"
        private const val kOnInterstitialAdClosed = "${kPrefix}OnInterstitialAdClosed"
    }

    private val _isLoaded = AtomicBoolean(false)

    val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    override fun onAdLoaded(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name} $_id")
            _isLoaded.set(true)
            _bridge.callCpp(kOnInterstitialAdLoaded)
        }
    }

    override fun onAdDisplayed(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdDisplayed.name}")
        }
    }

    override fun onAdHidden(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info(this::onAdHidden.name)
            _isLoaded.set(false)
            _bridge.callCpp(kOnInterstitialAdClosed)
        }
    }

    override fun onAdClicked(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdClicked.name}")
            _bridge.callCpp(kOnInterstitialAdClicked)
        }
    }

    override fun onAdLoadFailed(p0: String?, p1: MaxError?) {
        Thread.runOnMainThread {
            val errCode = p1?.code ?: 0;
            val errMsg = p1?.message ?: "";
            _logger.info("$kTag: ${this::onAdLoadFailed.name}: code $errCode")
            _bridge.callCpp(kOnInterstitialAdFailedToLoad, ErrorResponse(errCode, errMsg).serialize())
        }
    }

    override fun onAdDisplayFailed(p0: MaxAd?, p1: MaxError?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdDisplayFailed.name}")
        }
    }
}