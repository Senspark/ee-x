package com.ee.internal

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.IInterstitialAd
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.google.android.gms.ads.AdError
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.FullScreenContentCallback
import com.google.android.gms.ads.LoadAdError
import com.google.android.gms.ads.appopen.AppOpenAd
import com.google.common.truth.Truth.assertThat
import java.util.concurrent.atomic.AtomicBoolean

internal class AdMobAppOpenAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?,
    private val _adId: String)
    : IInterstitialAd {
    companion object {
        private val kTag = AdMobAppOpenAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobAppOpenAd", _adId)
    private val _helper = InterstitialAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: AppOpenAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
    }

    fun onCreate(activity: Activity) {
        _activity = activity
    }

    fun onDestroy(activity: Activity) {
        assertThat(_activity).isEqualTo(activity)
        _activity = null
    }

    @AnyThread
    fun destroy() {
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
            val callback = object : AppOpenAd.AppOpenAdLoadCallback() {
                override fun onAppOpenAdLoaded(ad: AppOpenAd) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAppOpenAdLoaded.name}: id = $_adId")
                        _isLoaded.set(true)
                        _ad = ad
                        _bridge.callCpp(_messageHelper.onLoaded)
                    }
                }

                override fun onAppOpenAdFailedToLoad(error: LoadAdError?) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: onAppOpenAdFailedToLoad: id = $_adId message = ${error?.message ?: ""} response = ${error?.responseInfo ?: ""}")
                        _bridge.callCpp(_messageHelper.onFailedToLoad, error?.message ?: "")
                    }
                }
            }
            val orientation = if (Utils.isLandscape(_application))
                AppOpenAd.APP_OPEN_AD_ORIENTATION_LANDSCAPE else
                AppOpenAd.APP_OPEN_AD_ORIENTATION_PORTRAIT
            AppOpenAd.load(_application, _adId, AdRequest.Builder().build(), orientation, callback)
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}: id = $_adId")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            val callback = object : FullScreenContentCallback() {
                override fun onAdShowedFullScreenContent() {
                    Thread.runOnMainThread {
                        _ad = null
                    }
                }

                override fun onAdFailedToShowFullScreenContent(error: AdError?) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdFailedToShowFullScreenContent.name}: id = $_adId message = ${error?.message ?: ""}")
                        _ad = null
                        _bridge.callCpp(_messageHelper.onFailedToShow, error?.message ?: "")
                    }
                }

                override fun onAdDismissedFullScreenContent() {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdDismissedFullScreenContent.name}: id = $_adId")
                        _ad = null
                        _bridge.callCpp(_messageHelper.onClosed)
                    }
                }
            }
            ad.show(_activity, callback)
        }
    }
}