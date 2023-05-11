package com.ee.internal

import androidx.annotation.AnyThread
import com.applovin.sdk.AppLovinAd
import com.applovin.sdk.AppLovinAdClickListener
import com.applovin.sdk.AppLovinAdDisplayListener
import com.applovin.sdk.AppLovinAdLoadListener
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinMaxInterstitialAdListener(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger)
    : AppLovinAdLoadListener, AppLovinAdDisplayListener, AppLovinAdClickListener {
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

    override fun adReceived(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::adReceived.name}")
            _isLoaded.set(true)
            _bridge.callCpp(kOnInterstitialAdLoaded)
        }
    }

    override fun failedToReceiveAd(errorCode: Int) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::failedToReceiveAd.name}: code $errorCode")
            _bridge.callCpp(kOnInterstitialAdFailedToLoad, ErrorResponse(errorCode, "").serialize())
        }
    }

    override fun adDisplayed(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::adDisplayed.name}")
        }
    }

    override fun adClicked(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::adClicked.name}")
            _bridge.callCpp(kOnInterstitialAdClicked)
        }
    }

    override fun adHidden(ad: AppLovinAd) {
        Thread.runOnMainThread {
            _logger.info(this::adHidden.name)
            _isLoaded.set(false)
            _bridge.callCpp(kOnInterstitialAdClosed)
        }
    }
}