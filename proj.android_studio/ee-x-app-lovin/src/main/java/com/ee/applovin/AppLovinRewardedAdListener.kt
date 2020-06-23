package com.ee.applovin

import com.applovin.sdk.AppLovinAd
import com.applovin.sdk.AppLovinAdClickListener
import com.applovin.sdk.AppLovinAdDisplayListener
import com.applovin.sdk.AppLovinAdLoadListener
import com.applovin.sdk.AppLovinAdRewardListener
import com.applovin.sdk.AppLovinAdVideoPlaybackListener
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinRewardedAdListener(
    private val _bridge: IMessageBridge,
    private val _isLoaded: AtomicBoolean)
    : AppLovinAdLoadListener
    , AppLovinAdDisplayListener
    , AppLovinAdClickListener
    , AppLovinAdRewardListener
    , AppLovinAdVideoPlaybackListener {
    companion object {
        private val _logger = Logger(AppLovinRewardedAdListener::class.java.name)

        private const val kPrefix = "AppLovin"
        private const val k__onRewardedAdLoaded = "${kPrefix}_onRewardedAdLoaded"
        private const val k__onRewardedAdFailedToLoad = "${kPrefix}_onRewardedAdFailedToLoad"
        private const val k__onRewardedAdClicked = "${kPrefix}_onRewardedAdClicked"
        private const val k__onRewardedAdClosed = "${kPrefix}_onRewardedAdClosed"
    }

    private var _rewarded = false

    override fun adReceived(ad: AppLovinAd) {
        _logger.info("${this::adReceived}")
        _isLoaded.set(true)
        _bridge.callCpp(k__onRewardedAdLoaded)
    }

    override fun failedToReceiveAd(errorCode: Int) {
        _logger.info("${this::failedToReceiveAd}: code $errorCode")
        _bridge.callCpp(k__onRewardedAdFailedToLoad, errorCode.toString())
    }

    override fun adDisplayed(ad: AppLovinAd) {
        _logger.info("${this::adDisplayed}")
        Thread.checkMainThread()
        _isLoaded.set(false)
    }

    override fun adHidden(ad: AppLovinAd) {
        _logger.info("${this::adHidden}")
        Thread.checkMainThread()
        _bridge.callCpp(k__onRewardedAdClosed, Utils.toString(_rewarded))
    }

    override fun userRewardVerified(ad: AppLovinAd, response: Map<String, String>) {
        _logger.info("${this::userRewardVerified}: $response")
        Thread.checkMainThread()
        _rewarded = true
    }

    override fun userOverQuota(ad: AppLovinAd, response: Map<String, String>) {
        _logger.info("${this::userOverQuota}: $response")
        Thread.checkMainThread()
    }

    override fun userRewardRejected(ad: AppLovinAd, response: Map<String, String>) {
        _logger.info("${this::userRewardRejected}: $response")
        Thread.checkMainThread()
    }

    override fun validationRequestFailed(ad: AppLovinAd, responseCode: Int) {
        _logger.info("${this::validationRequestFailed}: code = $responseCode")
        Thread.checkMainThread()
    }

    override fun userDeclinedToViewAd(ad: AppLovinAd) {
        _logger.info("${this::userDeclinedToViewAd}")
        Thread.checkMainThread()
    }

    override fun adClicked(ad: AppLovinAd) {
        _logger.info("${this::adClicked}")
        Thread.checkMainThread()
        _bridge.callCpp(k__onRewardedAdClicked)
    }

    override fun videoPlaybackBegan(ad: AppLovinAd) {
        _logger.info("${this::videoPlaybackBegan}")
        _rewarded = false
    }

    override fun videoPlaybackEnded(ad: AppLovinAd, percentViewed: Double, fullyWatched: Boolean) {
        _logger.info("${this::videoPlaybackEnded}")
    }
}