package com.ee.applovin

import android.app.Activity
import android.content.Context
import android.content.Intent
import com.applovin.adview.AppLovinIncentivizedInterstitial
import com.applovin.adview.AppLovinInterstitialAd
import com.applovin.adview.AppLovinInterstitialAdDialog
import com.applovin.sdk.AppLovinAdSize
import com.applovin.sdk.AppLovinSdk
import com.applovin.sdk.AppLovinSdkSettings
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.registerHandler

private class AppLovin(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(AppLovin::class.java.name)

        private const val kPrefix = "AppLovin"
        private const val k__initialize = "${kPrefix}_initialize"
        private const val k__setTestAdsEnabled = "${kPrefix}_setTestAdsEnabled"
        private const val k__setVerboseLogging = "${kPrefix}_setVerboseLogging"
        private const val k__setMuted = "${kPrefix}_setMuted"
        private const val k__hasInterstitialAd = "${kPrefix}_hasInterstitialAd"
        private const val k__loadInterstitialAd = "${kPrefix}_loadInterstitialAd"
        private const val k__showInterstitialAd = "${kPrefix}_showInterstitialAd"
        private const val k__hasRewardedAd = "${kPrefix}_hasRewardedAd"
        private const val k__loadRewardedAd = "${kPrefix}_loadRewardedAd"
        private const val k__showRewardedAd = "${kPrefix}_showRewardedAd"
    }

    private var _initialized = false
    private var _sdk: AppLovinSdk? = null
    private var _interstitialAd: AppLovinInterstitialAdDialog? = null
    private var _interstitialAdListener: AppLovinInterstitialAdListener? = null
    private var _rewardedAd: AppLovinIncentivizedInterstitial? = null
    private var _rewardedAdListener: AppLovinRewardedAdListener? = null

    init {
        _logger.debug("constructor begin: context = $_context")
        registerHandlers()
        _logger.debug("constructor end.")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {
        _activity = null
    }

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        return false
    }

    override fun destroy() {
        deregisterHandlers()
        if (!_initialized) {
            return
        }
        _sdk = null
        _interstitialAd?.let { ad ->
            ad.setAdLoadListener(null)
            ad.setAdDisplayListener(null)
            ad.setAdClickListener(null)
        }
        _interstitialAd = null
        _interstitialAdListener = null
        _rewardedAd = null
        _rewardedAdListener = null
    }

    private fun registerHandlers() {
        _bridge.registerHandler(k__initialize) { message ->
            initialize(message)
            ""
        }
        _bridge.registerHandler(k__setTestAdsEnabled) { message ->
            setTestAdEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__setVerboseLogging) { message ->
            setVerboseLogging(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__setMuted) { message ->
            setMuted(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__loadInterstitialAd) {
            loadInterstitialAd()
            ""
        }
        _bridge.registerHandler(k__hasInterstitialAd) {
            Utils.toString(hasInterstitialAd())
        }
        _bridge.registerHandler(k__showInterstitialAd) {
            showInterstitialAd()
            ""
        }
        _bridge.registerHandler(k__hasRewardedAd) {
            Utils.toString(hasRewardedAd())
        }
        _bridge.registerHandler(k__loadRewardedAd) {
            loadRewardedAd()
            ""
        }
        _bridge.registerHandler(k__showRewardedAd) {
            showRewardedAd()
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize)
        _bridge.deregisterHandler(k__setTestAdsEnabled)
        _bridge.deregisterHandler(k__setVerboseLogging)
        _bridge.deregisterHandler(k__setMuted)
        _bridge.deregisterHandler(k__hasInterstitialAd)
        _bridge.deregisterHandler(k__loadInterstitialAd)
        _bridge.deregisterHandler(k__showInterstitialAd)
        _bridge.deregisterHandler(k__hasRewardedAd)
        _bridge.deregisterHandler(k__loadRewardedAd)
        _bridge.deregisterHandler(k__showRewardedAd)
    }

    fun initialize(key: String) {
        Thread.checkMainThread()
        if (_initialized) {
            return
        }
        val settings = AppLovinSdkSettings()
        settings.setVerboseLogging(false)
        val sdk = AppLovinSdk.getInstance(key, settings, _context)
        sdk.initializeSdk()

        val interstitialAdListener = AppLovinInterstitialAdListener(_bridge)
        val interstitialAd = AppLovinInterstitialAd.create(sdk, _activity)
        interstitialAd.setAdLoadListener(interstitialAdListener)
        interstitialAd.setAdDisplayListener(interstitialAdListener)
        interstitialAd.setAdClickListener(interstitialAdListener)

        val rewardedAd = AppLovinIncentivizedInterstitial.create(sdk)
        val rewardedAdListener = AppLovinRewardedAdListener(_bridge)

        _sdk = sdk
        _interstitialAd = interstitialAd
        _interstitialAdListener = interstitialAdListener
        _rewardedAd = rewardedAd
        _rewardedAdListener = rewardedAdListener
        _initialized = true
    }

    fun setTestAdEnabled(enabled: Boolean) {
        Thread.checkMainThread()
        // Removed.
    }

    fun setVerboseLogging(enabled: Boolean) {
        Thread.checkMainThread()
        val sdk = _sdk ?: return
        sdk.settings.setVerboseLogging(enabled)
    }

    fun setMuted(enabled: Boolean) {
        Thread.checkMainThread()
        val sdk = _sdk ?: return
        sdk.settings.isMuted = enabled
    }

    fun hasInterstitialAd(): Boolean {
        Thread.checkMainThread()
        val ad = _interstitialAd ?: return false
        return ad.isAdReadyToDisplay
    }

    fun loadInterstitialAd() {
        Thread.checkMainThread()
        val sdk = _sdk ?: return
        sdk.adService.loadNextAd(AppLovinAdSize.INTERSTITIAL, _interstitialAdListener)
    }

    fun showInterstitialAd() {
        Thread.checkMainThread()
        val ad = _interstitialAd ?: return
        ad.show()
    }

    fun hasRewardedAd(): Boolean {
        Thread.checkMainThread()
        val ad = _rewardedAd ?: return false
        return ad.isAdReadyToDisplay
    }

    fun loadRewardedAd() {
        Thread.checkMainThread()
        val ad = _rewardedAd ?: return
        ad.preload(_rewardedAdListener)
    }

    fun showRewardedAd() {
        Thread.checkMainThread()
        val ad = _rewardedAd ?: return
        ad.show(_context, _rewardedAdListener, _rewardedAdListener, _rewardedAdListener, _rewardedAdListener)
    }
}