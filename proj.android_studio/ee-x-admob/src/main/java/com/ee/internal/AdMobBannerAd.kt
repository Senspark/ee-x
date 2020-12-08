package com.ee.internal

import android.app.Activity
import android.graphics.Point
import android.view.Gravity
import android.widget.FrameLayout
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IAdView
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils.getRootView
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.AdSize
import com.google.android.gms.ads.AdView
import com.google.android.gms.ads.LoadAdError
import com.google.common.truth.Truth
import kotlinx.serialization.InternalSerializationApi
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/13/17.
 */
@InternalSerializationApi
internal class AdMobBannerAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String,
    private val _adSize: AdSize,
    bannerHelper: AdMobBannerHelper)
    : IAdView, AdListener() {
    companion object {
        private val kTag = AdMobBannerAd::class.java.name
    }

    private val _messageHelper = MessageHelper("AdMobBannerAd", _adId)
    private val _helper = AdViewHelper(_bridge, this, _messageHelper)
    private val _viewHelper = ViewHelper(Point(0, 0), bannerHelper.getSize(_adSize), false)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: AdView? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
        createInternalAd()
    }

    @UiThread
    fun onCreate(activity: Activity) {
        _activity = activity
        addToActivity()
    }

    @UiThread
    fun onResume() {
        Thread.checkMainThread()
        _ad?.resume()
    }

    @UiThread
    fun onPause() {
        Thread.checkMainThread()
        _ad?.pause()
    }

    @UiThread
    fun onDestroy(activity: Activity) {
        Truth.assertThat(_activity).isEqualTo(activity)
        removeFromActivity()
        _activity = null
    }

    @AnyThread
    fun destroy() {
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

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            _isLoaded.set(false)
            val ad = AdView(_activity)
            ad.adSize = _adSize
            ad.adUnitId = _adId
            ad.adListener = this
            val params = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT)
            params.gravity = Gravity.START or Gravity.TOP
            ad.layoutParams = params
            _ad = ad
            _viewHelper.view = ad
            addToActivity()
        }
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread {
            val ad = _ad ?: return@runOnMainThread
            _isLoaded.set(false)
            removeFromActivity()
            ad.adListener = null
            ad.destroy()
            _ad = null
            _viewHelper.view = null
        }
    }

    @AnyThread
    private fun addToActivity() {
        Thread.runOnMainThread {
            val activity = _activity ?: return@runOnMainThread
            val rootView = getRootView(activity)
            rootView.addView(_ad)
        }
    }

    @AnyThread
    private fun removeFromActivity() {
        Thread.runOnMainThread {
            val activity = _activity ?: return@runOnMainThread
            val rootView = getRootView(activity)
            rootView.removeView(_ad)
        }
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::load.name}")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(AdRequest.Builder().build())
        }
    }

    override var position: Point
        @AnyThread get() = _viewHelper.position
        @AnyThread set(value) {
            _viewHelper.position = value
        }

    override var size: Point
        @AnyThread get() = _viewHelper.size
        @AnyThread set(value) {
            _viewHelper.size = value
        }

    override var isVisible: Boolean
        @AnyThread get() = _viewHelper.isVisible
        @AnyThread set(value) {
            _viewHelper.isVisible = value
        }

    override fun onAdLoaded() {
        _logger.debug("$kTag: ${this::onAdLoaded.name}")
        Thread.checkMainThread()
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdFailedToLoad(error: LoadAdError?) {
        _logger.debug("$kTag: onAdFailedToLoad: message = ${error?.message ?: ""} response = ${error?.responseInfo ?: ""}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, error?.message ?: "")
    }

    override fun onAdOpened() {
        _logger.debug("$kTag: ${this::onAdOpened.name}")
        Thread.checkMainThread()
    }

    override fun onAdImpression() {
        _logger.debug("$kTag: ${this::onAdImpression.name}")
        Thread.checkMainThread()
    }

    override fun onAdClicked() {
        _logger.debug("$kTag: ${this::onAdClicked.name}")
        Thread.checkMainThread()
    }

    override fun onAdLeftApplication() {
        _logger.debug("$kTag: ${this::onAdLeftApplication.name}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onAdClosed() {
        _logger.debug("$kTag: ${this::onAdClosed.name}")
        Thread.checkMainThread()
    }
}