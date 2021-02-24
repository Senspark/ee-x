package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.applovin.adview.AppLovinIncentivizedInterstitial
import com.applovin.adview.AppLovinInterstitialAd
import com.applovin.adview.AppLovinInterstitialAdDialog
import com.applovin.sdk.AppLovinAdSize
import com.applovin.sdk.AppLovinSdk
import com.applovin.sdk.AppLovinSdkSettings
import com.ee.internal.AppLovinInterstitialAdListener
import com.ee.internal.AppLovinRewardedAdListener
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.InternalSerializationApi
import kotlin.coroutines.resume

@InternalSerializationApi
class AppLovinBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = AppLovinBridge::class.java.name
        private const val kPrefix = "AppLovinBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kSetVerboseLogging = "${kPrefix}SetVerboseLogging"
        private const val kSetMuted = "${kPrefix}SetMuted"
        private const val kHasInterstitialAd = "${kPrefix}HasInterstitialAd"
        private const val kLoadInterstitialAd = "${kPrefix}LoadInterstitialAd"
        private const val kShowInterstitialAd = "${kPrefix}ShowInterstitialAd"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kLoadRewardedAd = "${kPrefix}LoadRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
    }

    private var _initializing = false
    private var _initialized = false
    private var _sdk: AppLovinSdk? = null
    private var _interstitialAd: AppLovinInterstitialAdDialog? = null
    private var _interstitialAdListener: AppLovinInterstitialAdListener? = null
    private var _rewardedAd: AppLovinIncentivizedInterstitial? = null
    private var _rewardedAdListener: AppLovinRewardedAdListener? = null

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
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
        Thread.runOnMainThread {
            if (!_initialized) {
                return@runOnMainThread
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
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            Utils.toString(initialize(message))
        }
        _bridge.registerHandler(kSetVerboseLogging) { message ->
            setVerboseLogging(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetMuted) { message ->
            setMuted(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kHasInterstitialAd) {
            Utils.toString(hasInterstitialAd)
        }
        _bridge.registerHandler(kLoadInterstitialAd) {
            loadInterstitialAd()
            ""
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
        _bridge.deregisterHandler(kSetVerboseLogging)
        _bridge.deregisterHandler(kSetMuted)
        _bridge.deregisterHandler(kHasInterstitialAd)
        _bridge.deregisterHandler(kLoadInterstitialAd)
        _bridge.deregisterHandler(kShowInterstitialAd)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
    }

    private fun checkInitialized() {
        if (!_initialized) {
            throw IllegalStateException("Please call initialize() first")
        }
    }

    @AnyThread
    suspend fun initialize(key: String): Boolean {
        return suspendCancellableCoroutine { cont ->
            Thread.runOnMainThread {
                if (_initializing) {
                    cont.resume(false)
                    return@runOnMainThread
                }
                if (_initialized) {
                    cont.resume(true)
                    return@runOnMainThread
                }
                _initializing = true
                val settings = AppLovinSdkSettings()
                val sdk = AppLovinSdk.getInstance(key, settings, _application)
                sdk.initializeSdk {
                    if (cont.isActive) {
                        // OK.
                    } else {
                        return@initializeSdk
                    }
                    Thread.runOnMainThread {
                        _initializing = false
                        _initialized = true
                        cont.resume(true)
                    }
                }

                val interstitialAdListener = AppLovinInterstitialAdListener(_bridge, _logger)
                val interstitialAd = AppLovinInterstitialAd.create(sdk, _activity)
                interstitialAd.setAdLoadListener(interstitialAdListener)
                interstitialAd.setAdDisplayListener(interstitialAdListener)
                interstitialAd.setAdClickListener(interstitialAdListener)

                val rewardedAd = AppLovinIncentivizedInterstitial.create(sdk)
                val rewardedAdListener = AppLovinRewardedAdListener(_bridge, _logger)

                _sdk = sdk
                _interstitialAd = interstitialAd
                _interstitialAdListener = interstitialAdListener
                _rewardedAd = rewardedAd
                _rewardedAdListener = rewardedAdListener
            }
        }
    }

    @AnyThread
    fun setVerboseLogging(enabled: Boolean) {
        Thread.runOnMainThread {
            checkInitialized()
            val sdk = _sdk ?: return@runOnMainThread
            sdk.settings.setVerboseLogging(enabled)
        }
    }

    @AnyThread
    fun setMuted(enabled: Boolean) {
        Thread.runOnMainThread {
            checkInitialized()
            val sdk = _sdk ?: return@runOnMainThread
            sdk.settings.isMuted = enabled
        }
    }

    private val hasInterstitialAd: Boolean
        @AnyThread get() = _interstitialAdListener?.isLoaded ?: false

    @AnyThread
    fun loadInterstitialAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val sdk = _sdk ?: return@runOnMainThread
            sdk.adService.loadNextAd(AppLovinAdSize.INTERSTITIAL, _interstitialAdListener)
        }
    }

    @AnyThread
    fun showInterstitialAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _interstitialAd ?: return@runOnMainThread
            ad.show()
        }
    }

    val hasRewardedAd: Boolean
        @AnyThread get() = _rewardedAdListener?.isLoaded ?: false

    @AnyThread
    fun loadRewardedAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _rewardedAd ?: return@runOnMainThread
            ad.preload(_rewardedAdListener)
        }
    }

    @AnyThread
    fun showRewardedAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _rewardedAd ?: return@runOnMainThread
            ad.show(_application, _rewardedAdListener, _rewardedAdListener, _rewardedAdListener, _rewardedAdListener)
        }
    }
}