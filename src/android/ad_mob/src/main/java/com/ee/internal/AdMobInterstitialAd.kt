package com.ee.internal

import android.app.Activity
import androidx.annotation.AnyThread
import com.ee.*
import com.google.android.gms.ads.*
import com.google.android.gms.ads.interstitial.InterstitialAd
import com.google.android.gms.ads.interstitial.InterstitialAdLoadCallback
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/13/17.
 */
internal class AdMobInterstitialAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String,
    private val _onAdPaid: (AdPaidResponse) -> Unit
) : IFullScreenAd {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )

    companion object {
        private val kTag = AdMobInterstitialAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobInterstitialAd", _adId)
    private val _helper = FullScreenAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onResume() {
    }

    override fun onPause() {
    }

    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
        _logger.info("$kTag: ${this::destroy.name}: adId = $_adId")
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _helper.registerHandlers()
    }

    @AnyThread
    private fun deregisterHandlers() {
        _helper.deregisterHandlers()
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::load.name}: id = $_adId")
            val activity = _activity
            if (activity == null) {
                _bridge.callCpp(_messageHelper.onFailedToLoad, "Null activity")
                return@runOnMainThread
            }
            val showCallback = object : FullScreenContentCallback() {
                override fun onAdShowedFullScreenContent() {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdShowedFullScreenContent.name}: id = $_adId")
                    }
                }

                override fun onAdFailedToShowFullScreenContent(error: AdError) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdFailedToShowFullScreenContent.name}: id = $_adId message = ${error.message}")
                        _isLoaded.set(false)
                        _ad = null
                        _bridge.callCpp(_messageHelper.onFailedToShow, ErrorResponse(error.code, error.message).serialize())
                    }
                }

                override fun onAdDismissedFullScreenContent() {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdDismissedFullScreenContent.name}: id = $_adId")
                        _isLoaded.set(false)
                        _ad = null
                        _bridge.callCpp(_messageHelper.onClosed)
                    }
                }
            }

            val onAdPaid = object : OnPaidEventListener {
                override fun onPaidEvent(adValue: AdValue) {
                    if (_ad == null) {
                        return;
                    }

                    _onAdPaid(
                        AdPaidResponse(
                            _ad!!.adUnitId,
                            "Interstitial",
                            adValue.valueMicros,
                            _ad!!.responseInfo.loadedAdapterResponseInfo
                        )
                    );
                }
            }

            val loadCallback = object : InterstitialAdLoadCallback() {
                override fun onAdLoaded(ad: InterstitialAd) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdLoaded.name}: id = $_adId")
                        ad.fullScreenContentCallback = showCallback
                        ad.onPaidEventListener = onAdPaid
                        _isLoaded.set(true)
                        _ad = ad
                        _bridge.callCpp(_messageHelper.onLoaded)
                    }
                }

                override fun onAdFailedToLoad(error: LoadAdError) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdFailedToLoad.name}: id = $_adId message = ${error.message} response = ${error.responseInfo ?: ""}")
                        _bridge.callCpp(_messageHelper.onFailedToLoad, ErrorResponse(error.code, error.message).serialize())
                    }
                }
            }
            InterstitialAd.load(activity, _adId, AdRequest.Builder().build(), loadCallback)
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}: id = $_adId")
            val ad = _ad
            if (ad == null) {
                _bridge.callCpp(_messageHelper.onFailedToShow, ErrorResponse(-1, "Null ad").serialize())
                return@runOnMainThread
            }
            val activity = _activity
            if (activity == null) {
                _bridge.callCpp(_messageHelper.onFailedToShow, ErrorResponse(-1, "Null activity").serialize())
                return@runOnMainThread
            }
            ad.show(activity)
        }
    }
}