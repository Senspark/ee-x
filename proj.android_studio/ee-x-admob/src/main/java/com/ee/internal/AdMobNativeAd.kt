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
import android.widget.RatingBar
import android.widget.TextView
import androidx.annotation.AnyThread
import androidx.annotation.UiThread
import com.ee.IAdView
import com.ee.IMessageBridge
import com.ee.Logger
import com.ee.Thread
import com.ee.Utils
import com.google.android.gms.ads.AdListener
import com.google.android.gms.ads.AdLoader
import com.google.android.gms.ads.AdRequest
import com.google.android.gms.ads.VideoController.VideoLifecycleCallbacks
import com.google.android.gms.ads.formats.MediaView
import com.google.android.gms.ads.formats.UnifiedNativeAd
import com.google.android.gms.ads.formats.UnifiedNativeAdView
import com.google.common.truth.Truth.assertThat
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import java.util.concurrent.atomic.AtomicBoolean

private typealias ViewProcessor<T> = (view: T) -> Unit

/**
 * Created by Zinge on 10/16/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
internal class AdMobNativeAd(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?,
    private val _adId: String,
    private val _layoutName: String,
    private val _identifiers: Map<String, String>)
    : IAdView
    , UnifiedNativeAd.OnUnifiedNativeAdLoadedListener
    , AdListener() {
    companion object {
        private val _logger = Logger(AdMobNativeAd::class.java.name)
        private const val k__body = "body"
        private const val k__call_to_action = "call_to_action"
        private const val k__headline = "headline"
        private const val k__icon = "icon"
        private const val k__image = "image"
        private const val k__media = "media"
        private const val k__price = "price"
        private const val k__star_rating = "star_rating"
        private const val k__store = "store"
    }

    private val _messageHelper = MessageHelper("AdMobNativeAd", _adId)
    private val _helper = AdViewHelper(_bridge, this, _messageHelper)
    private val _viewHelper = ViewHelper(Point(0, 0), Point(0, 0), false)
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: AdLoader? = null
    private var _view: UnifiedNativeAdView? = null

    init {
        _logger.info("constructor: adId = %s", _adId)
        registerHandlers()
        createInternalAd()
        createView()
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
            _ad = AdLoader.Builder(_context, _adId)
                .forUnifiedNativeAd(this)
                .withAdListener(this)
                .build()
        })
    }

    @AnyThread
    private fun destroyInternalAd() {
        Thread.runOnMainThread(Runnable {
            if (_ad == null) {
                return@Runnable
            }
            _isLoaded.set(false)
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
            _view = view as UnifiedNativeAdView
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

    @UiThread
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
            val ad = _ad ?: throw IllegalArgumentException("Ad is not initialized")
            ad.loadAd(AdRequest.Builder().build())
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

    override fun onUnifiedNativeAdLoaded(ad: UnifiedNativeAd) {
        _logger.info(this::onUnifiedNativeAdLoaded.name)
        val view = _view ?: throw IllegalArgumentException("Ad is not initialized")

        // Get the video controller for the ad. One will always be provided, even if the ad doesn't
        // have a video asset.
        val vc = ad.videoController

        // Create a new VideoLifecycleCallbacks object and pass it to the VideoController. The
        // VideoController will call methods on this object when events occur in the video
        // lifecycle.
        vc.videoLifecycleCallbacks = object : VideoLifecycleCallbacks() {
            override fun onVideoEnd() {
                // Publishers should allow native ads to complete video playback before refreshing
                // or replacing them with another ad in the same UI location.
                _logger.debug(this::onVideoEnd.name)
                super.onVideoEnd()
            }
        }

        // Some assets are guaranteed to be in every NativeAppInstallAd.
        processView<TextView>(view, k__body) { item ->
            view.bodyView = item
            item.text = ad.body
        }
        processView<Button>(view, k__call_to_action) { item ->
            view.callToActionView = item
            item.text = ad.callToAction
        }
        processView<TextView>(view, k__headline) { item ->
            view.headlineView = item
            item.text = ad.headline
        }
        processView<ImageView>(view, k__icon) { item ->
            view.iconView = item
            if (ad.icon != null) {
                item.setImageDrawable(ad.icon.drawable)
            }
        }

        // Apps can check the VideoController's hasVideoContent property to determine if the
        // NativeAppInstallAd has a video asset.
        if (vc.hasVideoContent()) {
            processView<ImageView>(view, k__image) { item ->
                item.visibility = View.GONE
            }
            processView<MediaView>(view, k__media) { item ->
                view.mediaView = item
            }
        } else {
            processView<ImageView>(view, k__image) { item ->
                val images = ad.images
                item.visibility = View.GONE
                for (image in images) {
                    if (image != null) {
                        item.visibility = View.VISIBLE
                        item.setImageDrawable(image.drawable)
                        break
                    }
                }
            }
            processView<MediaView>(view, k__media) { item ->
                item.visibility = View.GONE
            }
        }

        // These assets aren't guaranteed to be in every NativeAppInstallAd, so it's important to
        // check before trying to display them.
        processView<TextView>(view, k__price) { item ->
            view.priceView = item
            val price: CharSequence? = ad.price
            if (price != null) {
                item.visibility = View.VISIBLE
                item.text = price
            } else {
                item.visibility = View.INVISIBLE
            }
        }
        processView<RatingBar>(view, k__star_rating) { item ->
            view.starRatingView = item
            val starRating = ad.starRating
            if (starRating != null) {
                item.rating = starRating.toFloat()
                item.visibility = View.VISIBLE
            } else {
                item.visibility = View.INVISIBLE
            }
        }
        processView<TextView>(view, k__store) { item ->
            view.storeView = item
            val store: CharSequence? = ad.store
            if (store != null) {
                item.visibility = View.VISIBLE
                item.text = store
            } else {
                item.visibility = View.INVISIBLE
            }
        }

        // Assign native ad object to the native view.
        view.setNativeAd(ad)

        _isLoaded.set(true)
        _bridge.callCpp(_messageHelper.onLoaded)
    }

    override fun onAdLoaded() {
        _logger.info(this::onAdLoaded.name)
        Thread.checkMainThread()
        // There is one important difference between the way AdListener objects work with native ads
        // and the way they work with banners and interstitials. Because the AdLoader has its own
        // format-specific listeners (i.e., UnifiedNativeAd.OnUnifiedNativeAdLoadedListener) to use
        // when an ad has loaded, the onAdLoaded() method from AdListener is not called when a
        // native ad loads successfully.
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