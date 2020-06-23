package com.ee.admob

import android.content.Context
import androidx.annotation.AnyThread
import com.ee.ads.IInterstitialAd
import com.ee.ads.InterstitialAdHelper
import com.ee.ads.MessageHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.registerHandler
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.InterstitialAd
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/13/17.
 */
internal class AdMobInterstitialAd(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private val _adId: String)
    : AdListener()
    , IInterstitialAd {
    private val _messageHelper = MessageHelper("AdMobInterstitialAd", _adId)
    private val _helper = InterstitialAdHelper(_bridge, this, _messageHelper)

    companion object {
        private val _logger = Logger(AdMobInterstitialAd::class.java.name)
    }

    private val _isLoaded = AtomicBoolean(false)
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        registerHandlers()
        Thread.runOnMainThread(Runnable {
            createInternalAd()
        })
    }

    @AnyThread
    fun destroy() {
        _logger.info("${this::destroy}: adId = $_adId")
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            destroyInternalAd()
        })
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
            val ad = InterstitialAd(_context)
            ad.adUnitId = _adId
            ad.adListener = this
            _ad = ad
        })
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _ad ?: return@Runnable
            ad.adListener = null
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
            ad.loadAd(AdRequest.Builder().build())
        })
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread(Runnable {
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.show()
        })
    }

    override fun onAdClosed() {
        _logger.info("${this::onAdClosed}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClosed)
    }

    override fun onAdFailedToLoad(errorCode: Int) {
        _logger.info("${this::onAdFailedToLoad}: code = $errorCode")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, errorCode.toString())
    }

    override fun onAdLeftApplication() {
        _logger.info("${this::onAdLeftApplication}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onAdOpened() {
        _logger.info("${this::onAdOpened}")
        _isLoaded.set(false)
        Thread.checkMainThread()
    }

    override fun onAdLoaded() {
        _logger.info("${this::onAdLoaded}")
        Thread.checkMainThread()
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdClicked() {
        _logger.info("${this::onAdClosed}")
        Thread.checkMainThread()
        // https://stackoverflow.com/questions/47814295/interstitialad-listener-onadclicked-not-working
        // Use onAdLeftApplication instead.
    }

    override fun onAdImpression() {
        _logger.info("${this::onAdImpression}")
        Thread.checkMainThread()
    }
}