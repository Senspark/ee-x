package com.ee.internal

import android.app.Activity
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IFullScreenAd
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.InterstitialAd
import com.google.android.gms.ads.LoadAdError
import com.google.common.truth.Truth.assertThat
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/13/17.
 */
internal class AdMobInterstitialAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String)
    : IFullScreenAd, AdListener() {
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

    fun onCreate(activity: Activity) {
        _activity = activity
    }

    fun onDestroy(activity: Activity) {
        assertThat(_activity).isEqualTo(activity)
        _activity = null
    }

    @AnyThread
    fun destroy() {
        _logger.info("$kTag: ${this::destroy.name}: adId = $_adId")
        deregisterHandlers()
        destroyInternalAd()
    }

    @AnyThread
    private fun registerHandlers() {
        _helper.registerHandlers()
    }

    @AnyThread
    private fun deregisterHandlers() {
        _helper.deregisterHandlers()
    }

    @UiThread
    private fun createInternalAd(): InterstitialAd {
        _ad?.let {
            return@createInternalAd it
        }
        val ad = InterstitialAd(_activity)
        ad.adUnitId = _adId
        ad.adListener = this
        _ad = ad
        return ad
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread {
            val ad = _ad ?: return@runOnMainThread
            ad.adListener = null
            _ad = null
        }
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::load.name}: id = $_adId")
            val ad = createInternalAd()
            ad.loadAd(AdRequest.Builder().build())
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}: id = $_adId")
            val ad = createInternalAd()
            ad.show()
        }
    }

    override fun onAdLoaded() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name}: id = $_adId")
            _isLoaded.set(true)
            _bridge.callCpp(_messageHelper.onLoaded)
        }
    }

    override fun onAdFailedToLoad(error: LoadAdError?) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: onAdFailedToLoad: id = $_adId message = ${error?.message ?: ""} response = ${error?.responseInfo ?: ""}")
            destroyInternalAd()
            _bridge.callCpp(_messageHelper.onFailedToLoad, error?.message ?: "")
        }
    }

    override fun onAdOpened() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdOpened.name}: id = $_adId")
            _isLoaded.set(false)
        }
    }

    override fun onAdImpression() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdImpression.name}: id = $_adId")
        }
    }

    override fun onAdClicked() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdClosed.name}: id = $_adId")
            _bridge.callCpp(_messageHelper.onClicked)
        }
    }

    override fun onAdClosed() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdClosed.name}: id = $_adId")
            destroyInternalAd()
            _bridge.callCpp(_messageHelper.onClosed)
        }
    }
}