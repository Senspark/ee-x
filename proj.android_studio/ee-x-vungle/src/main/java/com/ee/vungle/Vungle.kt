package com.ee.vungle

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.internal.serialize
import com.ee.core.registerHandler
import com.vungle.warren.AdConfig
import com.vungle.warren.InitCallback
import com.vungle.warren.LoadAdCallback
import com.vungle.warren.PlayAdCallback
import com.vungle.warren.error.VungleException
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault
import java.util.*
import java.util.concurrent.ConcurrentHashMap

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
class Vungle(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(Vungle::class.java.name)

        private const val kPrefix = "Vungle"
        private const val k__initialize = "${kPrefix}_initialize"
        private const val k__hasRewardedAd = "${kPrefix}_hasRewardedAd"
        private const val k__loadRewardedAd = "${kPrefix}_loadRewardedAd"
        private const val k__showRewardedAd = "${kPrefix}_showRewardedAd"
        private const val k__onLoaded = "${kPrefix}_onLoaded"
        private const val k__onFailedToLoad = "${kPrefix}_onFailedToLoad"
        private const val k__onFailedToShow = "${kPrefix}_onFailedToShow"
        private const val k__onClosed = "${kPrefix}_onClosed"
    }

    private var _initializing = false
    private var _initialized = false
    private val _loadedAdIds: MutableSet<String> = Collections.newSetFromMap(ConcurrentHashMap())

    init {
        _logger.debug("constructor begin: context = $_context activity = $_activity")
        registerHandlers()
        _logger.debug("constructor end.")
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
        _bridge.registerHandler(k__initialize) { message ->
            @Serializable
            class Request(
                val gameId: String
            )

            val request = deserialize<Request>(message)
            initialize(request.gameId)
            ""
        }
        _bridge.registerHandler(k__hasRewardedAd) { message ->
            Utils.toString(hasRewardedAd(message))
        }
        _bridge.registerHandler(k__loadRewardedAd) { message ->
            loadRewardedAd(message)
            ""
        }
        _bridge.registerHandler(k__showRewardedAd) { message ->
            showRewardedAd(message)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize)
        _bridge.deregisterHandler(k__hasRewardedAd)
        _bridge.deregisterHandler(k__showRewardedAd)
        _bridge.deregisterHandler(k__loadRewardedAd)
    }

    @AnyThread
    fun initialize(gameId: String) {
        Thread.runOnMainThread(Runnable {
            if (_initializing) {
                return@Runnable
            }
            if (_initialized) {
                return@Runnable
            }
            _initializing = true

            com.vungle.warren.Vungle.init(gameId, _context, object : InitCallback {
                override fun onSuccess() {
                    _logger.info("${Vungle::initialize}: ${this::onSuccess}")
                    _initializing = false
                    _initialized = true
                }

                override fun onError(throwable: VungleException) {
                    _logger.info("${Vungle::initialize}: ${this::onError}: message = ${throwable.localizedMessage}")
                    _initializing = false
                }

                override fun onAutoCacheAdAvailable(adId: String) {}
            })
        })
    }

    @AnyThread
    private fun hasRewardedAd(adId: String): Boolean {
        return _loadedAdIds.contains(adId)
    }

    @AnyThread
    private fun loadRewardedAd(adId: String) {
        Thread.runOnMainThread(Runnable {
            com.vungle.warren.Vungle.loadAd(adId, object : LoadAdCallback {
                override fun onAdLoad(adId: String) {
                    _logger.info("${Vungle::loadRewardedAd}: ${this::onAdLoad}: $adId")
                    _loadedAdIds.add(adId)

                    @Serializable
                    @Suppress("unused")
                    class Response(
                        val ad_id: String
                    )

                    val response = Response(adId)
                    _bridge.callCpp(k__onLoaded, response.serialize())
                }

                override fun onError(adId: String, throwable: VungleException) {
                    _logger.info("${Vungle::loadRewardedAd}: ${this::onError}: $adId reason: ${throwable.localizedMessage}")
                    @Serializable
                    @Suppress("unused")
                    class Response(
                        val ad_id: String,
                        val message: String
                    )

                    val response = Response(adId, throwable.localizedMessage ?: "")
                    _bridge.callCpp(k__onFailedToLoad, response.serialize())
                }
            })
        })
    }

    @AnyThread
    private fun showRewardedAd(adId: String) {
        Thread.runOnMainThread(Runnable {
            com.vungle.warren.Vungle.playAd(adId, AdConfig(), object : PlayAdCallback {
                override fun onAdStart(adId: String) {
                    _logger.info("${Vungle::showRewardedAd}: ${this::onAdStart}: $adId")
                    _loadedAdIds.remove(adId)
                }

                override fun onAdEnd(adId: String, completed: Boolean, isCTAClicked: Boolean) {
                    _logger.info("${Vungle::showRewardedAd}: ${this::onAdEnd}: $adId successful = $completed clicked = $isCTAClicked")
                    @Serializable
                    @Suppress("unused")
                    class Response(
                        val ad_id: String,
                        val rewarded: Boolean
                    )

                    val response = Response(adId, completed)
                    _bridge.callCpp(k__onClosed, response.serialize())
                }

                override fun onError(adId: String, throwable: VungleException) {
                    _logger.info("${Vungle::showRewardedAd}: ${this::onError}: $adId message = ${throwable.localizedMessage}")
                    @Serializable
                    @Suppress("unused")
                    class Response(
                        val ad_id: String,
                        val message: String
                    )

                    val response = Response(adId, throwable.localizedMessage ?: "")
                    _bridge.callCpp(k__onFailedToShow, response.serialize())
                }
            })
        })
    }
}