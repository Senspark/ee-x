package com.ee.admob

import android.app.Activity
import android.content.Context
import android.graphics.Color
import android.graphics.Point
import android.view.Gravity
import android.widget.FrameLayout
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.ads.AdViewHelper
import com.ee.ads.IAdView
import com.ee.ads.MessageHelper
import com.ee.ads.ViewHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils.getRootView
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.AdSize
import com.google.android.gms.ads.AdView
import com.google.common.truth.Truth
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 10/13/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
internal class AdMobBannerAd(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?,
    private val _adId: String,
    private val _adSize: AdSize)
    : AdListener()
    , IAdView {
    companion object {
        private val _logger = Logger(AdMobBannerAd::class.java.name)
    }

    private val _messageHelper = MessageHelper("AdMobBannerAd", _adId)
    private val _helper = AdViewHelper(_bridge, this, _messageHelper)
    private var _isLoaded = false
    private var _ad: AdView? = null
    private var _viewHelper: ViewHelper? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        registerHandlers()
        Thread.runOnMainThread(Runnable {
            createInternalAd()
        })
    }

    fun onCreate(activity: Activity) {
        _activity = activity
        addToActivity()
    }

    fun onResume() {
        Thread.checkMainThread()
        _ad?.resume()
    }

    fun onPause() {
        Thread.checkMainThread()
        _ad?.pause()
    }

    fun onDestroy(activity: Activity) {
        Truth.assertThat(_activity).isEqualTo(activity)
        removeFromActivity()
        _activity = null
    }

    @AnyThread
    fun destroy() {
        _logger.info("${this::destroy}: adId = $_adId")
        deregisterHandlers()
        Thread.runOnMainThread(Runnable {
            destroyInternalAd()
        })
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
    private fun createInternalAd(): Boolean {
        Thread.checkMainThread()
        if (_ad != null) {
            return false
        }
        _isLoaded = false
        val ad = AdView(_context)
        ad.adSize = _adSize
        ad.adListener = this
        ad.adUnitId = _adId
        val params = FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT)
        params.gravity = Gravity.START or Gravity.TOP
        ad.layoutParams = params
        _ad = ad
        _viewHelper = ViewHelper(ad)
        addToActivity()
        return true
    }

    @UiThread
    private fun destroyInternalAd(): Boolean {
        Thread.checkMainThread()
        val ad = _ad ?: return false
        _isLoaded = false
        removeFromActivity()
        ad.destroy()
        _ad = null
        _viewHelper = null
        return true
    }

    @UiThread
    private fun addToActivity() {
        val activity = _activity ?: return
        val rootView = getRootView(activity)
        rootView.addView(_ad)
    }

    @UiThread
    private fun removeFromActivity() {
        val activity = _activity ?: return
        val rootView = getRootView(activity)
        rootView.removeView(_ad)
    }

    override val isLoaded: Boolean
        @UiThread get() {
            Thread.checkMainThread()
            Truth.assertThat(_ad).isNotNull()
            return _isLoaded
        }

    @UiThread
    override fun load() {
        _logger.info("${this::load}")
        Thread.checkMainThread()
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        ad.loadAd(AdRequest.Builder().build())
    }

    override var position: Point
        @UiThread get() = _viewHelper?.position ?: Point(0, 0)
        @UiThread set(value) {
            _viewHelper?.position = value
        }

    override var size: Point
        @UiThread get() = _viewHelper?.size ?: Point(0, 0)
        @UiThread set(value) {
            _viewHelper?.size = value
        }

    override var isVisible: Boolean
        @UiThread get() = _viewHelper?.isVisible ?: false
        @UiThread set(value) {
            _viewHelper?.isVisible = value
            if (value) {
                // https://stackoverflow.com/questions/21408178/admob-wont-show-the-banner-until
                // -refresh-or-sign-in-to-google-plus
                _ad?.setBackgroundColor(Color.BLACK)
            }
        }

    override fun onAdClosed() {
        _logger.info("${this::onAdClosed}")
        Thread.checkMainThread()
    }

    override fun onAdFailedToLoad(errorCode: Int) {
        _logger.info("${this::onAdFailedToLoad}: code = $errorCode")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, errorCode.toString())
    }

    override fun onAdLeftApplication() {
        _logger.info("${this::onAdLeftApplication}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onAdOpened() {
        _logger.info("${this::onAdOpened}")
        Thread.checkMainThread()
    }

    override fun onAdLoaded() {
        _logger.info("${this::onAdLoaded}")
        Thread.checkMainThread()
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdClicked() {
        _logger.info("${this::onAdClicked}")
        Thread.checkMainThread()
    }

    override fun onAdImpression() {
        _logger.info("${this::onAdImpression}")
        Thread.checkMainThread()
    }
}