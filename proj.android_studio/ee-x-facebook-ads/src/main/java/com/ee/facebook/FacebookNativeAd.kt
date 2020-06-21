package com.ee.facebook

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
import com.ee.ads.AdViewHelper
import com.ee.ads.IAdView
import com.ee.ads.MessageHelper
import com.ee.ads.ViewHelper
import com.ee.core.IMessageBridge
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
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
    : NativeAdListener
    , IAdView {
    companion object {
        private val _logger = Logger(FacebookNativeAd::class.java.name)

        private const val k__ad_choices = "ad_choices"
        private const val k__body = "body"
        private const val k__call_to_action = "call_to_action"
        private const val k__icon = "icon"
        private const val k__media = "media"
        private const val k__social_context = "social_context"
        private const val k__title = "title"
        private const val k__cover = "cover"
    }

    private val _messageHelper = MessageHelper("FacebookNativeAd", _adId)
    private val _helper = AdViewHelper(_bridge, this, _messageHelper)
    private var _isLoaded = false
    private var _ad: NativeAd? = null
    private var _view: View? = null
    private var _viewHelper: ViewHelper? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        Thread.checkMainThread()
        createInternalAd()
        createView()
        registerHandlers()
    }

    fun onCreate(activity: Activity) {
        _activity = activity
        addToActivity()
    }

    fun onDestroy(activity: Activity) {
        assertThat(_activity).isEqualTo(activity)
        removeFromActivity()
        _activity = null
    }

    fun destroy() {
        _logger.info("${this::destroy}: adId = $_adId")
        Thread.checkMainThread()
        deregisterHandlers()
        destroyView()
        destroyInternalAd()
    }

    @AnyThread
    private fun registerHandlers() {
        _helper.registerHandlers()
        _bridge.registerHandler(_messageHelper.createInternalAd) {
            Utils.toString(createInternalAd())
        }
        _bridge.registerHandler(_messageHelper.destroyInternalAd) {
            Utils.toString(destroyInternalAd())
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _helper.deregisterHandlers()
        _bridge.deregisterHandler(_messageHelper.createInternalAd)
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd)
    }

    @UiThread
    private fun createInternalAd(): Boolean {
        Thread.checkMainThread()
        if (_ad != null) {
            return false
        }
        _isLoaded = false
        _ad = NativeAd(_context, _adId)
        return true
    }

    @UiThread
    private fun destroyInternalAd(): Boolean {
        Thread.checkMainThread()
        val ad = _ad ?: return false
        _isLoaded = false
        ad.unregisterView()
        ad.destroy()
        _ad = null
        return true
    }

    @UiThread
    private fun createView() {
        Thread.checkMainThread()
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
        _viewHelper = ViewHelper(view)
        addToActivity()
    }

    @UiThread
    private fun destroyView() {
        Thread.checkMainThread()
        removeFromActivity()
        _view = null
        _viewHelper = null
    }

    @UiThread
    private fun addToActivity() {
        val activity = _activity ?: return
        val rootView = Utils.getRootView(activity)
        rootView.addView(_view)
    }

    @UiThread
    private fun removeFromActivity() {
        val activity = _activity ?: return
        val rootView = Utils.getRootView(activity)
        rootView.removeView(_view)
    }

    override val isLoaded: Boolean
        @UiThread get() {
            Thread.checkMainThread()
            assertThat(_ad).isNotNull()
            return _isLoaded
        }

    @UiThread
    override fun load() {
        _logger.info("${this::load}")
        Thread.checkMainThread()

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
        _logger.info("${this::onError}: ${adError.errorMessage}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onFailedToLoad, adError.errorMessage)
    }

    override fun onAdLoaded(nativeAd: Ad) {
        _logger.info("${this::onAdLoaded}")
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
        _isLoaded = true
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdClicked(ad: Ad) {
        _logger.info("${this::onAdClicked}")
        Thread.checkMainThread()
        _bridge.callCpp(_messageHelper.onClicked)
    }

    override fun onLoggingImpression(ad: Ad) {
        _logger.info("${this::onLoggingImpression}")
        Thread.checkMainThread()
    }

    override fun onMediaDownloaded(ad: Ad) {
        _logger.info("${this::onMediaDownloaded}")
    }
}