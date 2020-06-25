package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.applovin.adview.AppLovinIncentivizedInterstitial
import com.applovin.adview.AppLovinInterstitialAd
import com.applovin.adview.AppLovinInterstitialAdDialog
import com.applovin.sdk.AppLovinAdSize
import com.applovin.sdk.AppLovinSdk
import com.applovin.sdk.AppLovinSdkSettings
import com.ee.internal.AppLovinInterstitialAdListener
import com.ee.internal.AppLovinRewardedAdListener
import java.util.concurrent.atomic.AtomicBoolean

class AppLovinBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(AppLovinBridge::class.java.name)

        private const val kPrefix = "AppLovinBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetTestAdsEnabled = "${kPrefix}SetTestAdsEnabled"
        private const val kSetVerboseLogging = "${kPrefix}SetVerboseLogging"
        private const val kSetMuted = "${kPrefix}SetMuted"
        private const val kHasInterstitialAd = "${kPrefix}HasInterstitialAd"
        private const val kLoadInterstitialAd = "${kPrefix}LoadInterstitialAd"
        private const val kShowInterstitialAd = "${kPrefix}ShowInterstitialAd"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kLoadRewardedAd = "${kPrefix}LoadRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
    }

    private var _initialized = false
    private var _sdk: AppLovinSdk? = null
    private val _isInterstitialAdLoaded = AtomicBoolean(false)
    private var _interstitialAd: AppLovinInterstitialAdDialog? = null
    private var _interstitialAdListener: AppLovinInterstitialAdListener? = null
    private val _isRewardedAdLoaded = AtomicBoolean(false)
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

    override fun destroy() {
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            if (!_initialized) {
                return@Runnable
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
        })
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            initialize(message)
            ""
        }
        _bridge.registerHandler(kSetTestAdsEnabled) { message ->
            setTestAdEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetVerboseLogging) { message ->
            setVerboseLogging(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetMuted) { message ->
            setMuted(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kLoadInterstitialAd) {
            loadInterstitialAd()
            ""
        }
        _bridge.registerHandler(kHasInterstitialAd) {
            Utils.toString(hasInterstitialAd)
        }
        _bridge.registerHandler(kShowInterstitialAd) {
            showInterstitialAd()
            ""
        }
        _bridge.registerHandler(kHasRewardedAd) {
            Utils.toString(hasRewardedAd)
        }
        _bridge.registerHandler(kLoadRewardedAd) {
            loadRewardedAd()
            ""
        }
        _bridge.registerHandler(kShowRewardedAd) {
            showRewardedAd()
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kSetTestAdsEnabled)
        _bridge.deregisterHandler(kSetVerboseLogging)
        _bridge.deregisterHandler(kSetMuted)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    @AnyThread
    fun initialize(key: String) {
        Thread.runOnMainThread(Runnable {
            if (_initialized) {
                return@Runnable
            }
            val settings = AppLovinSdkSettings()
            settings.setVerboseLogging(false)
            val sdk = AppLovinSdk.getInstance(key, settings, _context)
            sdk.initializeSdk()

            val interstitialAdListener = AppLovinInterstitialAdListener(_bridge, _isInterstitialAdLoaded)
            val interstitialAd = AppLovinInterstitialAd.create(sdk, _activity)
            interstitialAd.setAdLoadListener(interstitialAdListener)
            interstitialAd.setAdDisplayListener(interstitialAdListener)
            interstitialAd.setAdClickListener(interstitialAdListener)

            val rewardedAd = AppLovinIncentivizedInterstitial.create(sdk)
            val rewardedAdListener = AppLovinRewardedAdListener(_bridge, _isRewardedAdLoaded)

            _sdk = sdk
            _interstitialAd = interstitialAd
            _interstitialAdListener = interstitialAdListener
            _rewardedAd = rewardedAd
            _rewardedAdListener = rewardedAdListener
            _initialized = true
        })
    }

    @AnyThread
    fun setTestAdEnabled(enabled: Boolean) {
        // Removed.
    }

    @AnyThread
    fun setVerboseLogging(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            val sdk = _sdk ?: throw IllegalStateException("Please call initialize() first")
            sdk.settings.setVerboseLogging(enabled)
        })
    }

    @AnyThread
    fun setMuted(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            val sdk = _sdk ?: throw IllegalStateException("Please call initialize() first")
            sdk.settings.isMuted = enabled
        })
    }

    val hasInterstitialAd: Boolean
        @AnyThread get() = _isInterstitialAdLoaded.get()

    @AnyThread
    fun loadInterstitialAd() {
        Thread.runOnMainThread(Runnable {
            val sdk = _sdk ?: throw IllegalStateException("Please call initialize() first")
            sdk.adService.loadNextAd(AppLovinAdSize.INTERSTITIAL, _interstitialAdListener)
        })
    }

    @AnyThread
    fun showInterstitialAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _interstitialAd
                ?: throw IllegalStateException("Please call initialize() first")
            ad.show()
        })
    }

    val hasRewardedAd: Boolean
        @AnyThread get() = _isRewardedAdLoaded.get()

    @AnyThread
    fun loadRewardedAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _rewardedAd ?: throw IllegalStateException("Please call initialize() first")
            ad.preload(_rewardedAdListener)
        })
    }

    @AnyThread
    fun showRewardedAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _rewardedAd ?: throw IllegalStateException("Please call initialize() first")
            ad.show(_context, _rewardedAdListener, _rewardedAdListener, _rewardedAdListener, _rewardedAdListener)
        })
    }
}