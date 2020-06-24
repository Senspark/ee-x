package com.ee.facebook

import android.content.Context
import androidx.annotation.AnyThread
import com.ee.ads.IInterstitialAd
import com.ee.ads.InterstitialAdHelper
import com.ee.ads.MessageHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.registerHandler
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
    private val _context: Context,
    private val _adId: String)
    : IInterstitialAd
    , InterstitialAdListener {
    companion object {
        private val _logger = Logger(FacebookInterstitialAd::class.java.name)
    }

    private val _messageHelper = MessageHelper("FacebookInterstitialAd", _adId)
    private val _helper = InterstitialAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        registerHandlers()
        createInternalAd()
    }

    @AnyThread
    fun destroy() {
        _logger.info("${this::destroy}: adId = $_adId")
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
        Thread.runOnMainThread(Runnable {
            if (_ad != null) {
                return@Runnable
            }
            _ad = InterstitialAd(_context, _adId)
        })
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _ad ?: return@Runnable
            ad.destroy()
            _ad = null
        })
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread(Runnable {
            _logger.info("${this::load}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(ad.buildLoadAdConfig().withAdListener(this).build())
        })
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread(Runnable {
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
            } else {
                _bridge.callCpp(_messageHelper.onFailedToShow)
            }
        })
    }

    override fun onInterstitialDisplayed(ad: Ad) {
        _logger.info("${this::onInterstitialDisplayed}")
        _isLoaded.set(false)
        Thread.checkMainThread()
    }

    override fun onInterstitialDismissed(ad: Ad) {
        _logger.info("${this::onInterstitialDismissed}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClosed)
    }

    override fun onError(ad: Ad, adError: AdError) {
        _logger.info("${this::onError}: ${adError.errorMessage}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
    }

    override fun onAdLoaded(ad: Ad) {
        _logger.info("${this::onAdLoaded}")
        Thread.checkMainThread()
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdClicked(ad: Ad) {
        _logger.info("${this::onAdClicked}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.info("${this::onLoggingImpression}")
        Thread.checkMainThread()
    }
}