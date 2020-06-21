package com.ee.facebook

import android.content.Context
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.ads.MessageHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.RewardedVideoAd
import com.facebook.ads.RewardedVideoAdListener

/**
 * Created by Zinge on 10/11/17.
 */
internal class FacebookRewardedAd(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private val _adId: String) : RewardedVideoAdListener {
    companion object {
        private val _logger = Logger(FacebookRewardedAd::class.java.name)
    }

    private val _messageHelper = MessageHelper("FacebookRewardedAd", _adId)
    private var _rewarded = false
    private var _ad: RewardedVideoAd? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        Thread.checkMainThread()
        createInternalAd()
        registerHandlers()
    }

    fun destroy() {
        _logger.info("destroy: adId = %s", _adId)
        Thread.checkMainThread()
        deregisterHandlers()
        destroyInternalAd()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(_messageHelper.createInternalAd) {
            Utils.toString(createInternalAd())
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) {
            Utils.toString(destroyInternalAd())
        }
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
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
        _bridge.deregisterHandler(_messageHelper.isLoaded)
        _bridge.deregisterHandler(_messageHelper.load)
        _bridge.deregisterHandler(_messageHelper.show)
    }

    @UiThread
    private fun createInternalAd(): Boolean {
        Thread.checkMainThread()
        if (_ad != null) {
            return false
        }
        _ad = RewardedVideoAd(_context, _adId)
        return true
    }

    @UiThread
    private fun destroyInternalAd(): Boolean {
        Thread.checkMainThread()
        val ad = _ad ?: return false
        ad.destroy()
        _ad = null
        return true
    }

    private val isLoaded: Boolean
        @UiThread get() {
            Thread.checkMainThread()
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            return ad.isAdLoaded
        }

    @UiThread
    private fun load() {
        _logger.info("${this::load}")
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        ad.loadAd(ad.buildLoadAdConfig().withAdListener(this).build())
    }

    @UiThread
    private fun show() {
        _logger.info("show")
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        _rewarded = false
        val result = ad.show(ad.buildShowAdConfig().build())
        if (result) {
            // OK.
        } else {
            _bridge.callCpp(_messageHelper.onFailedToShow)
        }
    }

    override fun onRewardedVideoCompleted() {
        _logger.info("${this::onRewardedVideoCompleted}")
        _rewarded = true
    }

    override fun onRewardedVideoClosed() {
        _logger.info("${this::onRewardedVideoClosed}")
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }

    override fun onAdClicked(ad: Ad) {
        _logger.info("${this::onAdClicked}")
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onError(ad: Ad, adError: AdError) {
        _logger.info("${this::onError}")
        _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
    }

    override fun onAdLoaded(ad: Ad) {
        _logger.info("${this::onAdLoaded}")
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.info("${this::onLoggingImpression}")
    }
}