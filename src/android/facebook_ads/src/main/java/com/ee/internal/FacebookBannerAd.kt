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
import com.ee.Utils
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.AdListener
import com.facebook.ads.AdSize
import com.facebook.ads.AdView
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.InternalSerializationApi
import java.util.concurrent.atomic.AtomicBoolean

/**
 * Created by Zinge on 10/9/17.
 */
@InternalSerializationApi
internal class FacebookBannerAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String,
    private val _adSize: AdSize,
    bannerHelper: FacebookBannerHelper)
    : IAdView, AdListener {
    companion object {
        private val kTag = FacebookBannerAd::class.java.name
    }

    private val _messageHelper = MessageHelper("FacebookBannerAd", _adId)
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
    fun onDestroy(activity: Activity) {
        assertThat(_activity).isEqualTo(activity)
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
            val ad = AdView(_activity, _adId, _adSize)
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
            ad.destroy()
            _ad = null
            _viewHelper.view = null
        }
    }

    @AnyThread
    private fun addToActivity() {
        Thread.runOnMainThread {
            val activity = _activity ?: return@runOnMainThread
            val rootView = Utils.getRootView(activity)
            rootView.addView(_ad)
        }
    }

    @AnyThread
    private fun removeFromActivity() {
        Thread.runOnMainThread {
            val activity = _activity ?: return@runOnMainThread
            val rootView = Utils.getRootView(activity)
            rootView.removeView(_ad)
        }
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread {
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(ad.buildLoadAdConfig().withAdListener(this).build())
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

    override fun onAdLoaded(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdLoaded.name}")
            _isLoaded.set(true)
            _bridge.callCpp(_messageHelper.onLoaded)
        }
    }

    override fun onError(ad: Ad, adError: AdError) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onError.name} ${adError.errorMessage}")
            _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
        }
    }

    override fun onLoggingImpression(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onLoggingImpression.name}")
        }
    }

    override fun onAdClicked(ad: Ad) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onAdClicked.name}")
            _bridge.callCpp(_messageHelper.onClicked)
        }
    }
}