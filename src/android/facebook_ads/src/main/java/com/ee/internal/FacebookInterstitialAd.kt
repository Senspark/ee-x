package com.ee.internal

import android.app.Activity
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IInterstitialAd
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
    : IInterstitialAd, InterstitialAdListener {
    companion object {
        private val kTag = FacebookInterstitialAd::class.java.name
    }

    private val _messageHelper = MessageHelper("FacebookInterstitialAd", _adId)
    private val _helper = InterstitialAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _displaying = false
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
        createInternalAd()
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

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            _ad = InterstitialAd(_activity, _adId)
        }
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
            _logger.info(this::load.name)
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(ad.buildLoadAdConfig()
                .withAdListener(this)
                .build())
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            _displaying = true
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
            } else {
                destroyInternalAd()
                createInternalAd()
                _bridge.callCpp(_messageHelper.onFailedToShow)
            }
        }
    }

    override fun onAdLoaded(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name}")
            _isLoaded.set(true)
            _bridge.callCpp(_messageHelper.onLoaded)
        }
    }

    override fun onError(ad: Ad, adError: AdError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onError.name}: ${adError.errorMessage}")
            destroyInternalAd()
            createInternalAd()
            if (_displaying) {
                _displaying = false
                _bridge.callCpp(_messageHelper.onFailedToShow, adError.errorMessage)
            } else {
                _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
            }
        }
    }

    override fun onInterstitialDisplayed(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialDisplayed.name}")
            _isLoaded.set(false)
        }
    }

    override fun onLoggingImpression(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onLoggingImpression.name}")
        }
    }

    override fun onAdClicked(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdClicked.name}")
            _bridge.callCpp(_messageHelper.onClicked)
        }
    }

    override fun onInterstitialDismissed(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onInterstitialDismissed.name}")
            _displaying = false
            destroyInternalAd()
            createInternalAd()
            _bridge.callCpp(_messageHelper.onClosed)
        }
    }
}