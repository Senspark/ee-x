package com.ee.internal

import android.app.Activity
import android.content.Context
import android.graphics.Point
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.widget.Button
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IAdView
import com.ee.IMessageBridge
import com.ee.Logger
import com.ee.Thread
import com.ee.Utils
import com.ee.registerHandler
import com.facebook.ads.Ad
import com.facebook.ads.AdError
import com.facebook.ads.AdOptionsView
import com.facebook.ads.MediaView
import com.facebook.ads.NativeAd
import com.facebook.ads.NativeAdBase
import com.facebook.ads.NativeAdListener
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import java.util.concurrent.atomic.AtomicBoolean

private typealias ViewProcessor<T> = (view: T) -> Unit

/**
 * Created by Zinge on 10/9/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
internal class FacebookNativeAd(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?,
    private val _adId: String,
    private val _layoutName: String,
    private val _identifiers: Map<String, String>)
    : IAdView
    , NativeAdListener {
    companion object {
        private val _logger = Logger(FacebookNativeAd::class.java.name)

        private const val k__ad_choices = "ad_choices"
        private const val k__body = "body"
        private const val k__call_to_action = "call_to_action"
        private const val k__icon = "icon"
        private const val k__media = "media"
        private const val k__social_context = "social_context"
        private const val k__title = "title"
    }

    private val _messageHelper = MessageHelper("FacebookNativeAd", _adId)
    private val _helper = AdViewHelper(_bridge, this, _messageHelper)
    private val _viewHelper = ViewHelper(Point(0, 0), Point(0, 0), false)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: NativeAd? = null
    private var _view: View? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        registerHandlers()
        createInternalAd()
        createView()
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
        _logger.info("${this::destroy.name}: adId = $_adId")
        deregisterHandlers()
        destroyView()
        destroyInternalAd()
    }

    @AnyThread
    private fun registerHandlers() {
        _helper.registerHandlers()
        _bridge.registerHandler(_messageHelper.createInternalAd) {
            createInternalAd()
            ""
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) {
            destroyInternalAd()
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _helper.deregisterHandlers()
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
    }

    @AnyThread
    private fun createInternalAd() {
        Thread.runOnMainThread(Runnable {
            if (_ad != null) {
                return@Runnable
            }
            _isLoaded.set(false)
            _ad = NativeAd(_context, _adId)
        })
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread(Runnable {
            val ad = _ad ?: return@Runnable
            _isLoaded.set(false)
            ad.unregisterView()
            ad.destroy()
            _ad = null
        })
    }

    @AnyThread
    private fun createView() {
        Thread.runOnMainThread(Runnable {
            val layoutId = _context.resources
                .getIdentifier(_layoutName, "layout", _context.packageName)
            val view = LayoutInflater
                .from(_context)
                .inflate(layoutId, null, false)
            view.visibility = View.INVISIBLE
            val params = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT)
            params.gravity = Gravity.START or Gravity.TOP
            view.layoutParams = params
            _view = view
            _viewHelper.view = view
            addToActivity()
        })
    }

    @AnyThread
    private fun destroyView() {
        Thread.runOnMainThread(Runnable {
            removeFromActivity()
            _view = null
            _viewHelper.view = null
        })
    }

    @AnyThread
    private fun addToActivity() {
        Thread.runOnMainThread(Runnable {
            val activity = _activity ?: return@Runnable
            val rootView = Utils.getRootView(activity)
            rootView.addView(_view)
        })
    }

    @AnyThread
    private fun removeFromActivity() {
        Thread.runOnMainThread(Runnable {
            val activity = _activity ?: return@Runnable
            val rootView = Utils.getRootView(activity)
            rootView.removeView(_view)
        })
    }

    override val isLoaded: Boolean
        @AnyThread get() = _isLoaded.get()

    @AnyThread
    override fun load() {
        Thread.runOnMainThread(Runnable {
            _logger.info(this::load.name)

            ///
            /// Audience Network supports pre-caching video or image assets which enables
            /// the MediaView to play videos immediately after nativeAd finishes loading.
            /// https://developers.facebook.com/docs/audience-network/android-native#mediaview
            ///
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(ad.buildLoadAdConfig()
                .withAdListener(this)
                .withMediaCacheFlag(NativeAdBase.MediaCacheFlag.ALL)
                .build())
        })
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

    private fun getIdentifier(identifier: String): Int {
        if (!_identifiers.containsKey(identifier)) {
            return 0
        }
        val resources = _context.resources
        return resources.getIdentifier(_identifiers[identifier], "id", _context.packageName)
    }

    @UiThread
    private fun <T : View> processView(view: View,
                                       key: String,
                                       processor: ViewProcessor<T>): Boolean {
        val id = getIdentifier(key)
        if (id == 0) {
            _logger.error("Can not find identifier for key: $key")
            return false
        }
        val subView = view.findViewById<View>(id)
        if (subView == null) {
            _logger.error("Can not find view for key: $key")
            return false
        }
        processor(subView as T)
        return true
    }

    override fun onError(ad: Ad, adError: AdError) {
        _logger.info("${this::onError.name}: ${adError.errorMessage}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
    }

    override fun onAdLoaded(nativeAd: Ad) {
        _logger.info(this::onAdLoaded.name)
        Thread.checkMainThread()
        assertThat(_ad === nativeAd)
        val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
        val view = _view ?: throw IllegalArgumentException("Ad is not initialized")
        ad.unregisterView()
        processView<Button>(view, k__call_to_action) { item ->
            item.visibility = if (ad.hasCallToAction()) View.VISIBLE else View.INVISIBLE
            item.text = ad.adCallToAction
        }
        processView<TextView>(view, k__title) { item ->
            item.text = ad.advertiserName
        }
        processView<TextView>(view, k__body) { item ->
            item.text = ad.adBodyText
        }
        processView<TextView>(view, k__social_context) { item ->
            item.text = ad.adSocialContext
        }
        processView<LinearLayout>(view, k__ad_choices) { item ->
            // Remove old icons.
            item.removeAllViews()

            // Add the AdChoices icon.
            val adOptionsView = AdOptionsView(_context, _ad, null)
            item.addView(adOptionsView)
        }
        processView<MediaView>(view, k__media) { item ->
            val callToAction = view.findViewById<Button>(getIdentifier(k__call_to_action))
            val icon = view.findViewById<ImageView>(getIdentifier(k__icon))
            ad.registerViewForInteraction(view, item, icon, listOf(callToAction, item))
        }
        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onMediaDownloaded(ad: Ad) {
        _logger.info(this::onMediaDownloaded.name)
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.info(this::onLoggingImpression.name)
        Thread.checkMainThread()
    }

    override fun onAdClicked(ad: Ad) {
        _logger.info(this::onAdClicked.name)
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }
}