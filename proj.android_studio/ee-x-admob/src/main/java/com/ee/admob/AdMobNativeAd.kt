package com.ee.admob

import android.app.Activity
import android.content.Context
import android.graphics.Point
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.RatingBar
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
    : AdListener()
    , IAdView {
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
    private val _isLoaded = AtomicBoolean(false)
    private var _ad: AdLoader? = null
    private var _view: FrameLayout? = null
    private var _viewHelper: ViewHelper? = null

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
        _logger.info("${this::destroy}: adId = $_adId")
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
                .forUnifiedNativeAd { nativeAppInstallAd ->
                    _logger.info("onAppInstallAdLoaded")
                    val layoutId = _context.resources.getIdentifier(_layoutName, "layout", _context.packageName)
                    val adView = LayoutInflater
                        .from(_context)
                        .inflate(layoutId, null, false) as UnifiedNativeAdView
                    populateUnifiedNativeAdView(nativeAppInstallAd, adView)
                    _view?.removeAllViews()
                    val params = ViewGroup.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT,
                        ViewGroup.LayoutParams.MATCH_PARENT)
                    _view?.addView(adView, params)
                    _isLoaded.set(true)
                    _bridge.callCpp(_messageHelper.onLoaded)
                }.withAdListener(this).build()
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
            assertThat(_view).isNull()
            val view = FrameLayout(_context)
            val params = FrameLayout.LayoutParams(
                FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT)
            params.gravity = Gravity.START or Gravity.TOP
            view.layoutParams = params
            _view = view
            _viewHelper = ViewHelper(view)
            addToActivity()
        })
    }

    @AnyThread
    private fun destroyView() {
        Thread.runOnMainThread(Runnable {
            removeFromActivity()
            _view = null
            _viewHelper = null
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
            _logger.info("${this::load}")
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
        }

    private fun getIdentifier(identifier: String): Int {
        if (!_identifiers.containsKey(identifier)) {
            return 0
        }
        val resources = _context.resources
        return resources.getIdentifier(_identifiers[identifier], "id", _context.packageName)
    }

    private fun populateUnifiedNativeAdView(nativeAppInstallAd: UnifiedNativeAd,
                                            adView: UnifiedNativeAdView) {
        // Get the video controller for the ad. One will always be provided, even if the ad doesn't
        // have a video asset.
        val vc = nativeAppInstallAd.videoController

        // Create a new VideoLifecycleCallbacks object and pass it to the VideoController. The
        // VideoController will call methods on this object when events occur in the video
        // lifecycle.
        vc.videoLifecycleCallbacks = object : VideoLifecycleCallbacks() {
            override fun onVideoEnd() {
                // Publishers should allow native ads to complete video playback before refreshing
                // or replacing them with another ad in the same UI location.
                _logger.debug("${this::onVideoEnd}")
                super.onVideoEnd()
            }
        }

        // Some assets are guaranteed to be in every NativeAppInstallAd.
        processView(adView, k__body) { item: TextView ->
            adView.bodyView = item
            item.text = nativeAppInstallAd.body
        }
        processView(adView, k__call_to_action) { item: Button ->
            adView.callToActionView = item
            item.text = nativeAppInstallAd.callToAction
        }
        processView(adView, k__headline) { item: TextView ->
            adView.headlineView = item
            item.text = nativeAppInstallAd.headline
        }
        processView(adView, k__icon) { item: ImageView ->
            adView.iconView = item
            if (nativeAppInstallAd.icon != null) {
                item.setImageDrawable(nativeAppInstallAd.icon.drawable)
            }
        }

        // Apps can check the VideoController's hasVideoContent property to determine if the
        // NativeAppInstallAd has a video asset.
        if (vc.hasVideoContent()) {
            processView<ImageView>(adView, k__image) { item ->
                item.visibility = View.GONE
            }
            processView(adView, k__media) { item: MediaView ->
                adView.mediaView = item
            }
        } else {
            processView<ImageView>(adView, k__image) { view ->
                val images = nativeAppInstallAd.images
                view.visibility = View.GONE
                for (image in images) {
                    if (image != null) {
                        view.visibility = View.VISIBLE
                        view.setImageDrawable(image.drawable)
                        break
                    }
                }
            }
            processView<MediaView>(adView, k__media) { view ->
                view.visibility = View.GONE
            }
        }

        // These assets aren't guaranteed to be in every NativeAppInstallAd, so it's important to
        // check before trying to display them.
        processView<TextView>(adView, k__price) { view ->
            adView.priceView = view
            val price: CharSequence? = nativeAppInstallAd.price
            if (price != null) {
                view.visibility = View.VISIBLE
                view.text = price
            } else {
                view.visibility = View.INVISIBLE
            }
        }
        processView<RatingBar>(adView, k__star_rating) { view ->
            adView.starRatingView = view
            val starRating = nativeAppInstallAd.starRating
            if (starRating != null) {
                view.rating = starRating.toFloat()
                view.visibility = View.VISIBLE
            } else {
                view.visibility = View.INVISIBLE
            }
        }
        processView<TextView>(adView, k__store) { view ->
            adView.storeView = view
            val store: CharSequence? = nativeAppInstallAd.store
            if (store != null) {
                view.visibility = View.VISIBLE
                view.text = store
            } else {
                view.visibility = View.INVISIBLE
            }
        }

        // Assign native ad object to the native view.
        adView.setNativeAd(nativeAppInstallAd)
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
        _logger.info(this::onAdOpened.name)
        Thread.checkMainThread()
    }

    override fun onAdLoaded() {
        _logger.info("${this::onAdLoaded}")
        Thread.checkMainThread()
        // There is one important difference between the way AdListener objects work with native ads
        // and the way they work with banners and interstitials. Because the AdLoader has its own
        // format-specific listeners (i.e., UnifiedNativeAd.OnUnifiedNativeAdLoadedListener) to use
        // when an ad has loaded, the onAdLoaded() method from AdListener is not called when a
        // native ad loads successfully.
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