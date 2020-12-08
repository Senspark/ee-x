package com.ee.internal

import android.app.Activity
import androidx.annotation.AnyThread
import com.ee.IInterstitialAd
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
    : IInterstitialAd, AdListener() {
    companion object {
        private val kTag = AdMobInterstitialAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobInterstitialAd", _adId)
    private val _helper = InterstitialAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: InterstitialAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
        createInternalAd()
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

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            val ad = InterstitialAd(_activity)
            ad.adUnitId = _adId
            ad.adListener = this
            _ad = ad
        }
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
            _logger.debug("$kTag: ${this::load.name}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(AdRequest.Builder().build())
        }
    }

    @AnyThread
    override fun show() {
        Thread.runOnMainThread {
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.show()
        }
    }

    override fun onAdLoaded() {
        _logger.debug("$kTag: ${this::onAdLoaded.name}")
        Thread.checkMainThread()
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdFailedToLoad(error: LoadAdError?) {
        _logger.debug("$kTag: onAdFailedToLoad: message = ${error?.message ?: ""} response = ${error?.responseInfo ?: ""}")
        Thread.checkMainThread()
        destroyInternalAd()
        createInternalAd()
        _bridge.callCpp(_messageHelper.onFailedToLoad, error?.message ?: "")
    }

    override fun onAdOpened() {
        _logger.debug("$kTag: ${this::onAdOpened.name}")
        Thread.checkMainThread()
        _isLoaded.set(false)
    }

    override fun onAdImpression() {
        _logger.debug("$kTag: ${this::onAdImpression.name}")
        Thread.checkMainThread()
    }

    override fun onAdClicked() {
        _logger.debug("$kTag: ${this::onAdClosed.name}")
        Thread.checkMainThread()
        // https://stackoverflow.com/questions/47814295/interstitialad-listener-onadclicked-not-working
        // Use onAdLeftApplication instead.
    }

    override fun onAdLeftApplication() {
        _logger.debug("$kTag: ${this::onAdLeftApplication.name}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onAdClosed() {
        _logger.debug("$kTag: ${this::onAdClosed.name}")
        Thread.checkMainThread()
        destroyInternalAd()
        createInternalAd()
        _bridge.callCpp(_messageHelper.onClosed)
    }
}