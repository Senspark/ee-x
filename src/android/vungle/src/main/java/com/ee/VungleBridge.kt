package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.ee.internal.deserialize
import com.ee.internal.serialize
import com.vungle.warren.AdConfig
import com.vungle.warren.InitCallback
import com.vungle.warren.LoadAdCallback
import com.vungle.warren.PlayAdCallback
import com.vungle.warren.Vungle
import com.vungle.warren.error.VungleException
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import java.util.Collections
import java.util.concurrent.ConcurrentHashMap
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@InternalSerializationApi
class VungleBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = VungleBridge::class.java.name
        private const val kPrefix = "VungleBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kHasRewardedAd = "${kPrefix}HasRewardedAd"
        private const val kLoadRewardedAd = "${kPrefix}LoadRewardedAd"
        private const val kShowRewardedAd = "${kPrefix}ShowRewardedAd"
        private const val kOnLoaded = "${kPrefix}OnLoaded"
        private const val kOnFailedToLoad = "${kPrefix}OnFailedToLoad"
        private const val kOnFailedToShow = "${kPrefix}OnFailedToShow"
        private const val kOnClicked = "${kPrefix}OnClicked"
        private const val kOnClosed = "${kPrefix}OnClosed"
    }

    private var _initializing = false
    private var _initialized = false
    private var _rewarded = false
    private val _loadedAdIds: MutableSet<String> = Collections.newSetFromMap(ConcurrentHashMap())

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end.")
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kInitialize) { message ->
            @Serializable
            class Request(
                val appId: String
            )

            val request = deserialize<Request>(message)
            Utils.toString(initialize(request.appId))
        }
        _bridge.registerHandler(kHasRewardedAd) { message ->
            Utils.toString(hasRewardedAd(message))
        }
        _bridge.registerHandler(kLoadRewardedAd) { message ->
            loadRewardedAd(message)
            ""
        }
        _bridge.registerHandler(kShowRewardedAd) { message ->
            showRewardedAd(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kHasRewardedAd)
        _bridge.deregisterHandler(kShowRewardedAd)
        _bridge.deregisterHandler(kLoadRewardedAd)
    }

    @AnyThread
    suspend fun initialize(appId: String): Boolean {
        return suspendCoroutine { cont ->
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
                Vungle.init(appId, _application, object : InitCallback {
                    override fun onSuccess() {
                        Thread.runOnMainThread {
                            _logger.debug("$kTag: initialize: ${this::onSuccess.name}")
                            _initializing = false
                            _initialized = true
                            cont.resume(true)
                        }
                    }

                    override fun onError(throwable: VungleException) {
                        Thread.runOnMainThread {
                            _logger.info("$kTag: initialize: ${this::onError.name}: message = ${throwable.localizedMessage}")
                            _initializing = false
                            cont.resume(false)
                        }
                    }

                    override fun onAutoCacheAdAvailable(adId: String) {}
                })
            }
        }
    }

    @AnyThread
    private fun hasRewardedAd(adId: String): Boolean {
        return _loadedAdIds.contains(adId)
    }

    @AnyThread
    private fun loadRewardedAd(adId: String) {
        Thread.runOnMainThread {
            Vungle.loadAd(adId, object : LoadAdCallback {
                override fun onAdLoad(adId: String) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::loadRewardedAd.name}: ${this::onAdLoad.name}: $adId")
                        _loadedAdIds.add(adId)

                        @Serializable
                        @Suppress("unused")
                        class Response(
                            val ad_id: String
                        )

                        val response = Response(adId)
                        _bridge.callCpp(kOnLoaded, response.serialize())
                    }
                }

                override fun onError(adId: String, exception: VungleException) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::loadRewardedAd.name}: ${this::onError.name}: $adId reason: ${exception.localizedMessage}")
                        @Serializable
                        @Suppress("unused")
                        class Response(
                            val ad_id: String,
                            val message: String
                        )

                        val response = Response(adId, exception.localizedMessage ?: "")
                        _bridge.callCpp(kOnFailedToLoad, response.serialize())
                    }
                }
            })
        }
    }

    @AnyThread
    private fun showRewardedAd(adId: String) {
        Thread.runOnMainThread {
            _rewarded = false
            Vungle.playAd(adId, AdConfig(), object : PlayAdCallback {
                override fun onError(adId: String, exception: VungleException) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: ${this::onError.name}: $adId message = ${exception.localizedMessage}")
                        @Serializable
                        @Suppress("unused")
                        class Response(
                            val ad_id: String,
                            val message: String
                        )

                        val response = Response(adId, exception.localizedMessage ?: "")
                        _bridge.callCpp(kOnFailedToShow, response.serialize())
                    }
                }

                override fun onAdViewed(id: String?) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: ${this::onAdViewed.name}: $adId")
                    }
                }

                override fun onAdStart(adId: String) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: ${this::onAdStart.name}: $adId")
                    }
                }

                override fun onAdClick(adId: String) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: ${this::onAdClick.name}: $adId")
                        _bridge.callCpp(kOnClicked, adId)
                    }
                }

                override fun onAdLeftApplication(adId: String) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: ${this::onAdLeftApplication.name}: $adId")
                    }
                }

                override fun onAdRewarded(adId: String) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: ${this::onAdRewarded.name}: $adId")
                        _rewarded = true
                    }
                }

                override fun onAdEnd(adId: String) {
                    Thread.runOnMainThread {
                        _logger.debug("$kTag: ${VungleBridge::showRewardedAd.name}: onAdEnd: $adId")
                        @Serializable
                        @Suppress("unused")
                        class Response(
                            val ad_id: String,
                            val rewarded: Boolean
                        )

                        _loadedAdIds.remove(adId)
                        val response = Response(adId, _rewarded)
                        _bridge.callCpp(kOnClosed, response.serialize())
                    }
                }

                override fun onAdEnd(adId: String, completed: Boolean, isCTAClicked: Boolean) {
                    // Deprecated.
                }
            })
        }
    }
}