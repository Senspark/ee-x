package com.ee.internal

import com.applovin.mediation.*
import com.ee.*
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinMaxRewardedAdListener(
    private val _id: String,
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _logAppsFlyerRevenue : (AdRevenueData) -> Unit
    )
    : MaxRewardedAdListener, MaxAdRevenueListener {
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
        private const val kOnRewardedAdPaid = "${kPrefix}OnRewardedAdPaid"
    }

    private val _isLoaded = AtomicBoolean(false)
    private var _rewarded = false

    val isLoaded: Boolean
        get() = _isLoaded.get()

    override fun onAdLoaded(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name} $_id")
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

    override fun onAdRevenuePaid(ad: MaxAd?) {
        if (ad == null) {
            return;
        }
        // Display name of the network that showed the ad (e.g. "AdColony")
        val networkName = ad.networkName
        val adUnitId = ad.adUnitId // The MAX Ad Unit ID
        // The ad format of the ad (e.g. BANNER, MREC, INTERSTITIAL, REWARDED)
        val adFormat = ad.format.getDisplayName();
        val revenue = ad.revenue // In USD

        val data = AdRevenueData(networkName, adUnitId, adFormat, revenue);

        _logger.info("$kTag: $networkName $adUnitId $adFormat $revenue")
        _bridge.callCpp(
            kOnRewardedAdPaid,
            data.serialize()
        )
        _logAppsFlyerRevenue(data)
    }
}