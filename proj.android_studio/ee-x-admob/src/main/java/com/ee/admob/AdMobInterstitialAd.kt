package com.ee.admob

import android.content.Context
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.ads.IInterstitialAd
import com.ee.ads.InterstitialAdHelper
import com.ee.ads.MessageHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.InterstitialAd

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

    private var _ad: InterstitialAd? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        createInternalAd()
        registerHandlers()
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
            Utils.toString(createInternalAd())
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) {
            Utils.toString(destroyInternalAd())
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _helper.deregisterHandlers()
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
    }

    @UiThread
    private fun createInternalAd(): Boolean {
        Thread.checkMainThread()
        if (_ad != null) {
            return false
        }
        val ad = InterstitialAd(_context)
        ad.adUnitId = _adId
        ad.adListener = this
        _ad = ad
        return true
    }

    @UiThread
    private fun destroyInternalAd(): Boolean {
        Thread.checkMainThread()
        val ad = _ad ?: return false
        ad.adListener = null
        _ad = null
        return true
    }

    override val isLoaded: Boolean
        @UiThread get() {
            Thread.checkMainThread()
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            return ad.isLoaded
        }

    @UiThread
    override fun load() {
        _logger.info("${this::load}")
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        ad.loadAd(AdRequest.Builder().build())
    }

    @UiThread
    override fun show() {
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        ad.show()
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
        Thread.checkMainThread()
    }

    override fun onAdLoaded() {
        _logger.info("${this::onAdLoaded}")
        Thread.checkMainThread()
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