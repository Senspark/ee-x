package com.ee.internal

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.google.android.gms.ads.AdError
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.rewarded.RewardItem
import com.google.android.gms.ads.rewarded.RewardedAd
import com.google.android.gms.ads.rewarded.RewardedAdCallback
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback
import com.google.common.truth.Truth.assertThat
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by KietLe on 12/03/19.
 */
internal class AdMobRewardedAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private var _activity: Activity?,
    private val _adId: String) : RewardedAdCallback() {
    companion object {
        private val kTag = AdMobRewardedAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobRewardedAd", _adId)
    private val _isLoaded = AtomicBoolean(false)
    private var _rewarded = false
    private var _ad: RewardedAd? = null

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
        _logger.info("$kTag: destroy: adId = $_adId")
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
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            _ad = RewardedAd(_context, _adId)
        }
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread {
            if (_ad == null) {
                return@runOnMainThread
            }
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
            val callback = object : RewardedAdLoadCallback() {
                override fun onRewardedAdLoaded() {
                    // Ad successfully loaded.
                    Thread.checkMainThread()
                    _isLoaded.set(true)
                    _bridge.callCpp(_messageHelper.onLoaded)
                }

                override fun onRewardedAdFailedToLoad(errorCode: Int) {
                    // Ad failed to load.
                    Thread.checkMainThread()
                    _bridge.callCpp(_messageHelper.onFailedToLoad, errorCode.toString())
                }
            }
            ad.loadAd(AdRequest.Builder().build(), callback)
        }
    }

    @AnyThread
    private fun show() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::show.name}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.show(_activity, this)
        }
    }

    override fun onRewardedAdFailedToShow(error: AdError?) {
        _logger.debug("$kTag: onRewardedAdFailedToShow: message = ${error?.message ?: ""}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToShow, error?.message ?: "")
    }

    override fun onRewardedAdOpened() {
        _logger.debug("$kTag: ${this::onRewardedAdOpened.name}")
        Thread.checkMainThread()
    }

    override fun onUserEarnedReward(reward: RewardItem) {
        _logger.debug("$kTag: ${this::onUserEarnedReward.name}")
        Thread.checkMainThread()
        _rewarded = true
    }

    override fun onRewardedAdClosed() {
        _logger.info("$kTag: ${this::onRewardedAdClosed.name}")
        Thread.checkMainThread()
        _isLoaded.set(false)
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }
}