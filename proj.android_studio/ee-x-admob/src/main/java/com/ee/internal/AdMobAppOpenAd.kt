package com.ee.internal

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.google.android.gms.ads.AdError
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.FullScreenContentCallback
import com.google.android.gms.ads.LoadAdError
import com.google.android.gms.ads.appopen.AppOpenAd
import com.google.android.gms.ads.appopen.AppOpenAd.AppOpenAdOrientation
import com.google.common.truth.Truth.assertThat
import java.util.concurrent.atomic.AtomicBoolean

internal class AdMobAppOpenAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?,
    private val _adId: String,
    @AppOpenAdOrientation private val _orientation: Int) {
    companion object {
        private val kTag = AdMobAppOpenAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobAppOpenAd", _adId)
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
        _bridge.registerHandler(_messageHelper.isLoaded) {
            Utils.toString(isLoaded)
        }
        _bridge.registerHandler(_messageHelper.load) {
            load()
            ""
        }
        _bridge.registerHandler(_messageHelper.show) {
            show()
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(_messageHelper.isLoaded)
        _bridge.deregisterHandler(_messageHelper.load)
        _bridge.deregisterHandler(_messageHelper.show)
    }

    private val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    private fun load() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::load.name}")
            val callback = object : AppOpenAd.AppOpenAdLoadCallback() {
                override fun onAppOpenAdLoaded(ad: AppOpenAd) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAppOpenAdLoaded.name}")
                        _isLoaded.set(true)
                        _ad = ad
                        _bridge.callCpp(_messageHelper.onLoaded)
                    }
                }

                override fun onAppOpenAdFailedToLoad(error: LoadAdError?) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: onAppOpenAdFailedToLoad: message = ${error?.message ?: ""} response = ${error?.responseInfo ?: ""}")
                        _bridge.callCpp(_messageHelper.onFailedToLoad, error?.message ?: "")
                    }
                }
            }
            AppOpenAd.load(_application, _adId, AdRequest.Builder().build(), _orientation, callback)
        }
    }

    @AnyThread
    private fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            val callback = object : FullScreenContentCallback() {
                override fun onAdShowedFullScreenContent() {
                    Thread.runOnMainThread {
                        _ad = null
                    }
                }

                override fun onAdFailedToShowFullScreenContent(error: AdError?) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${this::onAdFailedToShowFullScreenContent.name}: message = ${error?.message ?: ""}")
                        _ad = null
                        _bridge.callCpp(_messageHelper.onFailedToShow, error?.message ?: "")
                    }
                }

                override fun onAdDismissedFullScreenContent() {
                    Thread.runOnMainThread {
                        _logger.info("$kTag: ${this::onAdDismissedFullScreenContent.name}")
                        _ad = null
                        _bridge.callCpp(_messageHelper.onClosed)
                    }
                }
            }
            ad.show(_activity, callback)
        }
    }
}