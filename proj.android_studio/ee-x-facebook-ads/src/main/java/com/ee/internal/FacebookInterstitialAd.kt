package com.ee.internal

import android.content.Context
import androidx.annotation.AnyThread
import com.ee.IInterstitialAd
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.InterstitialAd
import com.facebook.ads.InterstitialAdListener
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/11/17.
 */
internal class FacebookInterstitialAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private val _adId: String)
    : IInterstitialAd, InterstitialAdListener {
    companion object {
        private val kTag = FacebookInterstitialAd::class.java.name
    }

    private val _messageHelper = MessageHelper("FacebookInterstitialAd", _adId)
    private val _helper = InterstitialAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
        createInternalAd()
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
        _bridge.registerHandler(_messageHelper.createInternalAd) {
            createInternalAd()
            ""
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) {
            destroyInternalAd()
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _helper.deregisterHandlers()
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
    }

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            _ad = InterstitialAd(_context, _adId)
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
            ad.loadAd(ad.buildLoadAdConfig().withAdListener(this).build())
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
            } else {
                _bridge.callCpp(_messageHelper.onFailedToShow)
            }
        }
    }

    override fun onAdLoaded(ad: Ad) {
        _logger.debug("$kTag: ${this::onAdLoaded.name}")
        Thread.checkMainThread()
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onError(ad: Ad, adError: AdError) {
        _logger.debug("$kTag: ${this::onError.name}: ${adError.errorMessage}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
    }

    override fun onInterstitialDisplayed(ad: Ad) {
        _logger.debug("$kTag: ${this::onInterstitialDisplayed.name}")
        Thread.checkMainThread()
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.debug("$kTag: ${this::onLoggingImpression.name}")
        Thread.checkMainThread()
    }

    override fun onAdClicked(ad: Ad) {
        _logger.debug("$kTag: ${this::onAdClicked.name}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onInterstitialDismissed(ad: Ad) {
        _logger.debug("$kTag: ${this::onInterstitialDismissed.name}")
        Thread.checkMainThread()
        _isLoaded.set(false)
        _bridge.callCpp(_messageHelper.onClosed)
    }
}