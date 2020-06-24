package com.ee.facebook

import android.content.Context
import androidx.annotation.AnyThread
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
import java.util.concurrent.atomic.AtomicBoolean

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
    private val _isLoaded = AtomicBoolean(false)
    private var _rewarded = false
    private var _ad: RewardedVideoAd? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        registerHandlers()
        createInternalAd()
    }

    @AnyThread
    fun destroy() {
        _logger.info("destroy: adId = %s", _adId)
        deregisterHandlers()
        destroyInternalAd()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(_messageHelper.createInternalAd) {
            createInternalAd()
            ""
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) {
            destroyInternalAd()
            ""
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

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread(Runnable {
            if (_ad != null) {
                return@Runnable
            }
            _ad = RewardedVideoAd(_context, _adId)
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

    private val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    private fun load() {
        Thread.runOnMainThread(Runnable {
            _logger.info(this::load.name)
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(ad.buildLoadAdConfig().withAdListener(this).build())
        })
    }

    @AnyThread
    private fun show() {
        Thread.runOnMainThread(Runnable {
            _logger.info(this::show.name)
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            _rewarded = false
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
            } else {
                _bridge.callCpp(_messageHelper.onFailedToShow)
            }
        })
    }

    override fun onError(ad: Ad, adError: AdError) {
        _logger.info("${this::onError.name}: ${adError.errorMessage}")
        _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
    }

    override fun onAdLoaded(ad: Ad) {
        _logger.info(this::onAdLoaded.name)
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.info(this::onLoggingImpression.name)
    }

    override fun onAdClicked(ad: Ad) {
        _logger.info(this::onAdClicked.name)
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onRewardedVideoCompleted() {
        _logger.info(this::onRewardedVideoCompleted.name)
        _rewarded = true
    }

    override fun onRewardedVideoClosed() {
        _logger.info(this::onRewardedVideoClosed.name)
        _isLoaded.set(false)
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }
}