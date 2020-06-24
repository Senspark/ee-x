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
import java.util.concurrent.atomic.AtomicBoolean

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
    : IAdView
    , AdListener() {
    companion object {
        private val _logger = Logger(AdMobBannerAd::class.java.name)
    }

    private val _messageHelper = MessageHelper("AdMobBannerAd", _adId)
    private val _helper = AdViewHelper(_bridge, this, _messageHelper)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: AdView? = null
    private var _viewHelper: ViewHelper? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
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
        _logger.info("${this::destroy.name}: adId = $_adId")
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
        Thread.runOnMainThread(Runnable {
            if (_ad != null) {
                return@Runnable
            }
            _isLoaded.set(false)
            val ad = AdView(_context)
            ad.adSize = _adSize
            ad.adUnitId = _adId
            ad.adListener = this
            val params = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT)
            params.gravity = Gravity.START or Gravity.TOP
            ad.layoutParams = params
            _ad = ad
            _viewHelper = ViewHelper(ad)
            addToActivity()
        })
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _ad ?: return@Runnable
            _isLoaded.set(false)
            removeFromActivity()
            ad.adListener = null
            ad.destroy()
            _ad = null
            _viewHelper = null
        })
    }

    @AnyThread
    private fun addToActivity() {
        Thread.runOnMainThread(Runnable {
            val activity = _activity ?: return@Runnable
            val rootView = getRootView(activity)
            rootView.addView(_ad)
        })
    }

    @AnyThread
    private fun removeFromActivity() {
        Thread.runOnMainThread(Runnable {
            val activity = _activity ?: return@Runnable
            val rootView = getRootView(activity)
            rootView.removeView(_ad)
        })
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread(Runnable {
            _logger.info(this::load.name)
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(AdRequest.Builder().build())
        })
    }

    override var position: Point
        @AnyThread get() = _viewHelper?.position ?: Point(0, 0)
        @AnyThread set(value) {
            _viewHelper?.position = value
        }

    override var size: Point
        @AnyThread get() = _viewHelper?.size ?: Point(0, 0)
        @AnyThread set(value) {
            _viewHelper?.size = value
        }

    override var isVisible: Boolean
        @AnyThread get() = _viewHelper?.isVisible ?: false
        @AnyThread set(value) {
            _viewHelper?.isVisible = value
            if (value) {
                // https://stackoverflow.com/questions/21408178/admob-wont-show-the-banner-until
                // -refresh-or-sign-in-to-google-plus
                _ad?.setBackgroundColor(Color.BLACK)
            }
        }

    override fun onAdLoaded() {
        _logger.info(this::onAdLoaded.name)
        Thread.checkMainThread()
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdFailedToLoad(errorCode: Int) {
        _logger.info("${this::onAdFailedToLoad.name}: code = $errorCode")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, errorCode.toString())
    }

    override fun onAdOpened() {
        _logger.info(this::onAdOpened.name)
        Thread.checkMainThread()
    }

    override fun onAdImpression() {
        _logger.info(this::onAdImpression.name)
        Thread.checkMainThread()
    }

    override fun onAdClicked() {
        _logger.info(this::onAdClicked.name)
        Thread.checkMainThread()
    }

    override fun onAdLeftApplication() {
        _logger.info(this::onAdLeftApplication.name)
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onAdClosed() {
        _logger.info(this::onAdClosed.name)
        Thread.checkMainThread()
    }
}