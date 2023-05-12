package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.applovin.mediation.ads.MaxAdView
import com.applovin.mediation.ads.MaxInterstitialAd
import com.applovin.mediation.ads.MaxRewardedAd
import com.applovin.sdk.AppLovinSdk
import com.ee.internal.AppLovinMaxBannerAdListener
import com.ee.internal.AppLovinMaxInterstitialAdListener
import com.ee.internal.AppLovinMaxRewardedAdListener
import com.ee.internal.deserialize
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.Serializable
import kotlin.coroutines.resume

class AppLovinMaxBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {

    @Serializable
    private class InitializeRequest(
        val bannerAdId: String,
        val rewardedAdId: String,
        val interstitialAdId: String,
    )
    companion object {
        private val kTag = AppLovinMaxBridge::class.java.name
        private const val kPrefix = "AppLovinMaxBridge"
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
//    private var _adView: MaxAdView? = null
//    private var _adViewListener: AppLovinMaxBannerAdListener? = null;
    private var _interstitialAd: MaxInterstitialAd? = null
    private var _interstitialAdListener: AppLovinMaxInterstitialAdListener? = null
    private var _rewardedAd: MaxRewardedAd? = null
    private var _rewardedAdListener: AppLovinMaxRewardedAdListener? = null

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
            _interstitialAd = null
            _interstitialAdListener = null
            _rewardedAd = null
            _rewardedAdListener = null
        }
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            val request = deserialize<InitializeRequest>(message)
            Utils.toString(
                initialize(
                    request.bannerAdId,
                    request.rewardedAdId,
                    request.interstitialAdId
                )
            )
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
    suspend fun initialize(
        bannerAdId: String,
        rewardedAdId: String,
        interstitialAdId: String
    ): Boolean {
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

                val sdk = AppLovinSdk.getInstance(_application);
                sdk.mediationProvider = "max";
                sdk.initializeSdk {
                    if (cont.isActive) {
                        // OK.
                        sdk.showMediationDebugger()
                    } else {
                        return@initializeSdk
                    }
                    Thread.runOnMainThread {
                        _initializing = false
                        _initialized = true
                        cont.resume(true)
                    }
                };
//
//                val adView = MaxAdView(bannerAdId, _activity);
//                val adViewListener = AppLovinMaxBannerAdListener(_bridge, _logger);
//                adView.setListener(adViewListener);

                val interstitialAd = MaxInterstitialAd(interstitialAdId, _activity)
                val interstitialAdListener = AppLovinMaxInterstitialAdListener(_bridge, _logger)
                interstitialAd.setListener(interstitialAdListener)

                val rewardedAd = MaxRewardedAd.getInstance(rewardedAdId, _activity)
                val rewardedAdListener = AppLovinMaxRewardedAdListener(_bridge, _logger)
                rewardedAd.setListener(rewardedAdListener);

                _sdk = sdk
//                _adView = adView;
//                _adViewListener = adViewListener;
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
        @AnyThread get() = _interstitialAd?.isReady ?: false

    @AnyThread
    fun loadInterstitialAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _interstitialAd ?: return@runOnMainThread
            ad.loadAd()
        }
    }

    @AnyThread
    fun showInterstitialAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _interstitialAd ?: return@runOnMainThread
            ad.showAd()
        }
    }

    val hasRewardedAd: Boolean
        @AnyThread get() = _rewardedAd?.isReady ?: false

    @AnyThread
    fun loadRewardedAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _rewardedAd ?: return@runOnMainThread
            ad.loadAd()
        }
    }

    @AnyThread
    fun showRewardedAd() {
        Thread.runOnMainThread {
            checkInitialized()
            val ad = _rewardedAd ?: return@runOnMainThread
            ad.showAd()
        }
    }
}