package com.ee.internal

import android.app.Activity
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IFullScreenAd
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.RewardedVideoAd
import com.facebook.ads.RewardedVideoAdListener
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/11/17.
 */
internal class FacebookRewardedAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String)
    : IFullScreenAd, RewardedVideoAdListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )

    companion object {
        private val kTag = FacebookRewardedAd::class.java.name
    }

    private val _messageHelper = MessageHelper("FacebookRewardedAd", _adId)
    private val _helper = FullScreenAdHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _displaying = false
    private var _rewarded = false
    private var _ad: RewardedVideoAd? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onResume() {
    }

    override fun onPause() {
    }

    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
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
    private fun createInternalAd(): RewardedVideoAd {
        _ad?.let {
            return@createInternalAd it
        }
        val ad = RewardedVideoAd(_activity, _adId)
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
                .withFailOnCacheFailureEnabled(true)
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
            _rewarded = false
            val result = ad.show(ad.buildShowAdConfig().build())
            if (result) {
                // OK.
                _isLoaded.set(false)
            } else {
                destroyInternalAd()
                _bridge.callCpp(_messageHelper.onFailedToShow, ErrorResponse(-1, "Failed to show ad").serialize())
            }
        }
    }

    override fun onError(ad: Ad, error: AdError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onError.name}: id = $_adId message = ${error.errorMessage}")
            destroyInternalAd()
            if (_displaying) {
                _displaying = false
                _isLoaded.set(false)
                _bridge.callCpp(_messageHelper.onFailedToShow, ErrorResponse(error.errorCode, error.errorMessage).serialize())
            } else {
                _bridge.callCpp(_messageHelper.onFailedToLoad, ErrorResponse(error.errorCode, error.errorMessage).serialize())
            }
        }
    }

    override fun onAdLoaded(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name}: id = $_adId")
            _isLoaded.set(true)
            _bridge.callCpp(_messageHelper.onLoaded)
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

    override fun onRewardedVideoCompleted() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoCompleted.name}: id = $_adId")
            _rewarded = true
        }
    }

    override fun onRewardedVideoClosed() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onRewardedVideoClosed.name}: id = $_adId")
            _displaying = false
            _isLoaded.set(false)
            destroyInternalAd()
            _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
        }
    }
}