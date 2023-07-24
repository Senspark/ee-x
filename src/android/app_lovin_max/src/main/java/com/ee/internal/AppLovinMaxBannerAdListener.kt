package com.ee.internal

import android.view.ViewGroup
import com.applovin.mediation.*
import com.applovin.mediation.ads.MaxAdView
import com.ee.AdRevenueData
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AppLovinMaxBannerAdListener(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _logAppsFlyerRevenue: (AdRevenueData) -> Unit
) : MaxAdViewAdListener, MaxAdRevenueListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )
    companion object {
        private val kTag = AppLovinMaxBannerAdListener::class.java.name
        private const val kPrefix = "AppLovinMaxBridge"
        private const val kOnBannerAdLoaded = "${kPrefix}OnBannerAdLoaded"
        private const val kOnBannerAdFailedToLoad = "${kPrefix}OnBannerAdFailedToLoad"
        private const val kOnBannerAdClicked = "${kPrefix}OnBannerAdClicked"
        private const val kOnBannerAdClosed = "${kPrefix}OnBannerAdClosed"
        private const val kOnBannerAdPaid = "${kPrefix}OnBannerAdPaid"
    }

    private val _isLoaded = AtomicBoolean(false)

    val isLoaded: Boolean
        get() = _isLoaded.get()

    override fun onAdLoaded(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name}")
            _isLoaded.set(true)
//            _bridge.callCpp(kOnBannerAdLoaded)
        }
    }

    override fun onAdDisplayed(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdDisplayed.name}")
        }
    }

    override fun onAdHidden(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdHidden.name}")
        }
    }

    override fun onAdClicked(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdClicked.name}")
        }
    }

    override fun onAdLoadFailed(p0: String?, p1: MaxError?) {
        Thread.runOnMainThread {
            val errCode = p1?.code ?: 0;
            val errMsg = p1?.message ?: "";
            _logger.info("$kTag: ${this::onAdLoadFailed.name}: code $errCode msg: $errMsg")
//            _bridge.callCpp(
//                kOnBannerAdFailedToLoad, ErrorResponse(errCode, errMsg).serialize()
//            )
        }
    }

    override fun onAdDisplayFailed(p0: MaxAd?, p1: MaxError?) {
        Thread.runOnMainThread {
            _logger.info("$kTag: ${this::onAdDisplayFailed.name}")
        }
    }

    override fun onAdExpanded(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdExpanded.name}")
        }
    }

    override fun onAdCollapsed(p0: MaxAd?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdCollapsed.name}")
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

        _logger.info("${kTag}: $networkName $adUnitId $adFormat $revenue")
        _bridge.callCpp(
            kOnBannerAdPaid,
            data.serialize()
        )
        _logAppsFlyerRevenue(data)
    }
}