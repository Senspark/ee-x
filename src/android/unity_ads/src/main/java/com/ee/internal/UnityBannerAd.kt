package com.ee.internal

import android.app.Activity
import android.graphics.Point
import android.view.Gravity
import android.widget.FrameLayout
import androidx.annotation.AnyThread
import com.ee.IBannerAd
import com.ee.ILogger
import com.ee.IMessageBridge
import com.ee.Thread
import com.ee.Utils
import com.unity3d.services.banners.BannerErrorInfo
import com.unity3d.services.banners.BannerView
import com.unity3d.services.banners.UnityBannerSize
import kotlinx.serialization.Serializable
import java.util.concurrent.atomic.AtomicBoolean

class UnityBannerAd(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private var _activity: Activity?,
    private val _adId: String,
    private val _adSize: UnityBannerSize,
    bannerHelper: UnityBannerHelper)
    : IBannerAd, BannerView.IListener {
    @Serializable
    @Suppress("unused")
    private class ErrorResponse(
        val code: Int,
        val message: String
    )

    companion object {
        private val kTag = UnityBannerAd::class.java.name
    }

    private val _messageHelper = MessageHelper("UnityBannerAd", _adId)
    private val _helper = BannerAdHelper(_bridge, this, _messageHelper)
    private val _viewHelper = ViewHelper(Point(0, 0), bannerHelper.getSize(_adSize), false)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: BannerView? = null

    init {
        _logger.info("$kTag: constructor: adId = $_adId")
        registerHandlers()
        createInternalAd()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        addToActivity()
    }

    override fun onResume() {
    }

    override fun onPause() {
    }

    override fun onDestroy() {
        removeFromActivity()
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

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread {
            if (_ad != null) {
                return@runOnMainThread
            }
            _isLoaded.set(false)
            val ad = BannerView(_activity, _adId, _adSize)
            ad.listener = this
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
            ad.listener = null
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
            _logger.debug("$kTag: ${this::load.name}: id = $_adId")
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.load()
        }
    }

    override var isVisible: Boolean
        @AnyThread get() = _viewHelper.isVisible
        @AnyThread set(value) {
            _viewHelper.isVisible = value
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

    override fun onBannerLoaded(bannerAdView: BannerView) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onBannerLoaded.name}: id = $_adId")
            _isLoaded.set(true)
            _bridge.callCpp(_messageHelper.onLoaded)
        }
    }

    override fun onBannerFailedToLoad(bannerAdView: BannerView, error: BannerErrorInfo) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onBannerFailedToLoad.name}: id = $_adId message = ${error.errorMessage}")
            _bridge.callCpp(_messageHelper.onFailedToLoad, ErrorResponse(error.errorCode.ordinal, error.errorMessage).serialize())
        }
    }

    override fun onBannerClick(bannerAdView: BannerView) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onBannerClick.name}: id = $_adId")
            _bridge.callCpp(_messageHelper.onClicked)
        }
    }

    override fun onBannerLeftApplication(bannerView: BannerView) {
        Thread.runOnMainThread {
            _logger.debug("$kTag: ${this::onBannerLeftApplication.name}: id = $_adId")
        }
    }
}