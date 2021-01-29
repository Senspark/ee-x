package com.ee.internal

import android.app.Activity
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IFullScreenAd
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.InterstitialAd
import com.facebook.ads.InterstitialAdListener
import com.google.common.truth.Truth.assertThat
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/11/17.
 */
internal class FacebookInterstitialAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String)
    : IFullScreenAd, InterstitialAdListener {
    companion object {
        private val kTag = FacebookInterstitialAd::class.java.name
    }

    private val _messageHelper = MessageHelper("FacebookInterstitialAd", _adId)
    private val _helper = FullScreenAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _displaying = false
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
    }

    @UiThread
    fun onCreate(activity: Activity) {
        _activity = activity
    }

    @UiThread
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
        val ad = InterstitialAd(_activity, _adId)
        _ad = ad
        return ad
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread {
            val ad = _ad ?: return@runOnMainThread
            ad.destroy()
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
            ad.loadAd(ad.buildLoadAdConfig()
                .withAdListener(this)
                .build())
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}: id = $_adId")
            val ad = createInternalAd()
            _displaying = true
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
            } else {
                _isLoaded.set(false)
                destroyInternalAd()
                _bridge.callCpp(_messageHelper.onFailedToShow)
            }
        }
    }

    override fun onAdLoaded(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name} id = $_adId")
            _isLoaded.set(true)
            _bridge.callCpp(_messageHelper.onLoaded)
        }
    }

    override fun onError(ad: Ad, adError: AdError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onError.name}: id = $_adId message = ${adError.errorMessage}")
            destroyInternalAd()
            if (_displaying) {
                _displaying = false
                _isLoaded.set(false)
                _bridge.callCpp(_messageHelper.onFailedToShow, adError.errorMessage)
            } else {
                _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
            }
        }
    }

    override fun onInterstitialDisplayed(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialDisplayed.name}: id = $_adId")
        }
    }

    override fun onLoggingImpression(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onLoggingImpression.name}: id = $_adId")
        }
    }

    override fun onAdClicked(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdClicked.name}: id = $_adId")
            _bridge.callCpp(_messageHelper.onClicked)
        }
    }

    override fun onInterstitialDismissed(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialDismissed.name}: id = $_adId")
            _displaying = false
            _isLoaded.set(false)
            destroyInternalAd()
            _bridge.callCpp(_messageHelper.onClosed)
        }
    }
}