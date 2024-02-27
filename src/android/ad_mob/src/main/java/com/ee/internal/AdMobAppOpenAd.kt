package com.ee.internal

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.*
import com.google.android.gms.ads.*
import com.google.android.gms.ads.appopen.AppOpenAd
import com.google.android.gms.ads.appopen.AppOpenAd.AppOpenAdLoadCallback
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

internal class AdMobAppOpenAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
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
        private val kTag = AdMobAppOpenAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobAppOpenAd", _adId)
    private val _helper = FullScreenAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: AppOpenAd? = null

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
        _logger.info("$kTag: destroy: adId = $_adId")
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
                            "App Open",
                            adValue.valueMicros,
                            _ad!!.responseInfo.loadedAdapterResponseInfo
                        )
                    );
                }
            }

            val loadCallback = object : AppOpenAdLoadCallback() {
                override fun onAdLoaded(ad: AppOpenAd) {
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
            val orientation = if (Utils.isLandscape(_application))
                AppOpenAd.APP_OPEN_AD_ORIENTATION_LANDSCAPE else
                AppOpenAd.APP_OPEN_AD_ORIENTATION_PORTRAIT
            AppOpenAd.load(_application, _adId, AdRequest.Builder().build(), orientation, loadCallback)
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