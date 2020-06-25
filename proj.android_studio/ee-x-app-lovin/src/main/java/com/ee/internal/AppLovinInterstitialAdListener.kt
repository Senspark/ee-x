package com.ee.internal

import androidx.annotation.AnyThread
import com.applovin.sdk.AppLovinAd
import com.applovin.sdk.AppLovinAdClickListener
import com.applovin.sdk.AppLovinAdDisplayListener
import com.applovin.sdk.AppLovinAdLoadListener
import com.ee.IMessageBridge
import com.ee.Logger
import com.ee.Thread
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinInterstitialAdListener(
    private val _bridge: IMessageBridge)
    : AppLovinAdLoadListener
    , AppLovinAdDisplayListener
    , AppLovinAdClickListener {
    companion object {
        private val _logger = Logger(AppLovinInterstitialAdListener::class.java.name)

        private const val kPrefix = "AppLovinBridge"
        private const val kOnInterstitialAdLoaded = "${kPrefix}OnInterstitialAdLoaded"
        private const val kOnInterstitialAdFailedToLoad = "${kPrefix}OnInterstitialAdFailedToLoad"
        private const val kOnInterstitialAdClicked = "${kPrefix}OnInterstitialAdClicked"
        private const val kOnInterstitialAdClosed = "${kPrefix}OnInterstitialAdClosed"
    }

    private val _isLoaded = AtomicBoolean(false)

    val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    override fun adReceived(ad: AppLovinAd) {
        _logger.info(this::adReceived.name)
        _isLoaded.set(true)
        _bridge.callCpp(kOnInterstitialAdLoaded)
    }

    override fun failedToReceiveAd(errorCode: Int) {
        _logger.info("${this::failedToReceiveAd.name}: code $errorCode")
        _bridge.callCpp(kOnInterstitialAdFailedToLoad, errorCode.toString())
    }

    override fun adDisplayed(ad: AppLovinAd) {
        _logger.info(this::adDisplayed.name)
        Thread.checkMainThread()
    }

    override fun adClicked(ad: AppLovinAd) {
        _logger.info(this::adClicked.name)
        Thread.checkMainThread()
        _bridge.callCpp(kOnInterstitialAdClicked)
    }

    override fun adHidden(ad: AppLovinAd) {
        _logger.info(this::adHidden.name)
        Thread.checkMainThread()
        _isLoaded.set(false)
        _bridge.callCpp(kOnInterstitialAdClosed)
    }
}