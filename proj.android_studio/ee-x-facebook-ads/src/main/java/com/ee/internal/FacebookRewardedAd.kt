package com.ee.internal

import android.content.Context
import androidx.annotation.AnyThread
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.RewardedVideoAd
import com.facebook.ads.RewardedVideoAdListener
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/11/17.
 */
internal class FacebookRewardedAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private val _adId: String) : RewardedVideoAdListener {
    companion object {
        private val kTag = FacebookRewardedAd::class.java.name
    }

    private val _messageHelper = MessageHelper("FacebookRewardedAd", _adId)
    private val _isLoaded = AtomicBoolean(false)
    private var _displaying = false
    private var _rewarded = false
    private var _ad: RewardedVideoAd? = null

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

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            _ad = RewardedVideoAd(_context, _adId)
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

    private val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    private fun load() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::load.name}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(ad.buildLoadAdConfig().withAdListener(this).build())
        }
    }

    @AnyThread
    private fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            _displaying = true
            _rewarded = false
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
                _isLoaded.set(false)
            } else {
                destroyInternalAd()
                createInternalAd()
                _bridge.callCpp(_messageHelper.onFailedToShow)
            }
        }
    }

    override fun onError(ad: Ad, adError: AdError) {
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

    override fun onAdLoaded(ad: Ad) {
        _logger.debug("$kTag: ${this::onAdLoaded.name}")
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.debug("$kTag: ${this::onLoggingImpression.name}")
    }

    override fun onAdClicked(ad: Ad) {
        _logger.debug("$kTag: ${this::onAdClicked.name}")
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onRewardedVideoCompleted() {
        _logger.debug("$kTag: ${this::onRewardedVideoCompleted.name}")
        _rewarded = true
    }

    override fun onRewardedVideoClosed() {
        _logger.info(this::onRewardedVideoClosed.name)
        _displaying = false
        destroyInternalAd()
        createInternalAd()
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }
}