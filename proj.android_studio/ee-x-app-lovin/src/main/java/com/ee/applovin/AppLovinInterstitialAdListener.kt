package com.ee.applovin

import com.applovin.sdk.AppLovinAd
import com.applovin.sdk.AppLovinAdClickListener
import com.applovin.sdk.AppLovinAdDisplayListener
import com.applovin.sdk.AppLovinAdLoadListener
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinInterstitialAdListener(
    private val _bridge: IMessageBridge,
    private val _isLoaded: AtomicBoolean)
    : AppLovinAdLoadListener
    , AppLovinAdDisplayListener
    , AppLovinAdClickListener {
    companion object {
        private val _logger = Logger(AppLovinInterstitialAdListener::class.java.name)

        private const val kPrefix = "AppLovin"
        private const val k__onInterstitialAdLoaded = "${kPrefix}_onInterstitialAdLoaded"
        private const val k__onInterstitialAdFailedToLoad = "${kPrefix}_onInterstitialAdFailedToLoad"
        private const val k__onInterstitialAdClicked = "${kPrefix}_onInterstitialAdClicked"
        private const val k__onInterstitialAdClosed = "${kPrefix}_onInterstitialAdClosed"
    }

    override fun adReceived(ad: AppLovinAd) {
        _logger.info(this::adReceived.name)
        _isLoaded.set(true)
        _bridge.callCpp(k__onInterstitialAdLoaded)
    }

    override fun failedToReceiveAd(errorCode: Int) {
        _logger.info("${this::failedToReceiveAd.name}: code $errorCode")
        _bridge.callCpp(k__onInterstitialAdFailedToLoad, errorCode.toString())
    }

    override fun adDisplayed(ad: AppLovinAd) {
        _logger.info(this::adDisplayed.name)
        Thread.checkMainThread()
    }

    override fun adClicked(ad: AppLovinAd) {
        _logger.info(this::adClicked.name)
        Thread.checkMainThread()
        _bridge.callCpp(k__onInterstitialAdClicked)
    }

    override fun adHidden(ad: AppLovinAd) {
        _logger.info(this::adHidden.name)
        Thread.checkMainThread()
        _isLoaded.set(false)
        _bridge.callCpp(k__onInterstitialAdClosed)
    }
}