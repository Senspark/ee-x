package com.ee.admob

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.ads.MessageHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.rewarded.RewardItem
import com.google.android.gms.ads.rewarded.RewardedAd
import com.google.android.gms.ads.rewarded.RewardedAdCallback
import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback
import com.google.common.truth.Truth.assertThat

/**
 * Created by KietLe on 12/03/19.
 */
internal class AdMobRewardedAd(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?,
    private val _adId: String) : RewardedAdCallback() {
    companion object {
        private val _logger = Logger(AdMobRewardedAd::class.java.name)
    }

    private val _messageHelper = MessageHelper("AdMobRewardedAd", _adId)
    private var _rewarded = false
    private var _ad: RewardedAd? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        createInternalAd()
        Thread.runOnMainThread(Runnable {
            registerHandlers()
        })
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
        _logger.info("destroy: adId = %s", _adId)
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            destroyInternalAd()
        })
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
        _ad = RewardedAd(_context, _adId)
        return true
    }

    @UiThread
    private fun destroyInternalAd(): Boolean {
        Thread.checkMainThread()
        if (_ad == null) {
            return false
        }
        _ad = null
        return true
    }

    private val isLoaded: Boolean
        @UiThread get() {
            Thread.checkMainThread()
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            return ad.isLoaded
        }

    @UiThread
    private fun load() {
        _logger.info("${this::load}")
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        val callback = object : RewardedAdLoadCallback() {
            override fun onRewardedAdLoaded() {
                // Ad successfully loaded.
                Thread.checkMainThread()
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

    @UiThread
    private fun show() {
        _logger.info("${this::show}")
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        ad.show(_activity, this)
    }

    override fun onRewardedAdOpened() {
        Thread.checkMainThread()
        _logger.info("${this::onRewardedAdOpened}")
    }

    override fun onRewardedAdClosed() {
        _logger.info("${this::onRewardedAdClosed}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClosed, Utils.toString(_rewarded))
    }

    override fun onUserEarnedReward(reward: RewardItem) {
        _logger.info("${this::onUserEarnedReward}")
        Thread.checkMainThread()
        _rewarded = true
    }

    override fun onRewardedAdFailedToShow(errorCode: Int) {
        _logger.info("${this::onRewardedAdFailedToShow}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToShow, errorCode.toString())
    }
}