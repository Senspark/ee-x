package com.ee.google.analytics

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.Thread
import com.ee.core.internal.Utils
import com.ee.core.internal.deserialize
import com.ee.core.registerHandler
import com.google.android.gms.analytics.HitBuilders.EventBuilder
import com.google.android.gms.analytics.HitBuilders.ExceptionBuilder
import com.google.android.gms.analytics.HitBuilders.ScreenViewBuilder
import com.google.android.gms.analytics.HitBuilders.SocialBuilder
import com.google.android.gms.analytics.HitBuilders.TimingBuilder
import com.google.android.gms.analytics.ecommerce.Product
import com.google.android.gms.analytics.ecommerce.ProductAction
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import java.util.concurrent.ConcurrentHashMap

/**
 * Created by Zinge on 10/13/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
class GoogleAnalytics(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(GoogleAnalytics::class.java.name)

        private const val k__setDispatchInterval = "GoogleAnalytics_setDispatchInterval"
        private const val k__setDryRun = "GoogleAnalytics_setDryRun"
        private const val k__setOptOut = "GoogleAnalytics_setOptOut"
        private const val k__setTrackUncaughtException = "GoogleAnalytics_setTrackUncaughtException"
        private const val k__dispatch = "GoogleAnalytics_dispatch"
        private const val k__createTracker = "GoogleAnalytics_createTracker"
        private const val k__destroyTracker = "GoogleAnalytics_destroyTracker"
        private const val k__testTrackEvent = "GoogleAnalytics_testTrackEvent"
        private const val k__testTrackException = "GoogleAnalytics_testTrackException"
        private const val k__testTrackScreenView = "GoogleAnalytics_testTrackScreenView"
        private const val k__testTrackSocial = "GoogleAnalytics_testTrackSocial"
        private const val k__testTrackTiming = "GoogleAnalytics_testTrackTiming"
        private const val k__testCustomDimensionAndMetric = "GoogleAnalytics_testCustomDimensionAndMetric"
        private const val k__testTrackEcommerceAction = "GoogleAnalytics_testTrackEcommerceAction"
        private const val k__testTrackEcommerceImpression = "GoogleAnalytics_testTrackEcommerceImpression"
    }

    private val _analytics = com.google.android.gms.analytics.GoogleAnalytics.getInstance(_context)
    private val _trackers: MutableMap<String, GoogleAnalyticsTracker> = ConcurrentHashMap()
    private var _exceptionReportingEnabled = false

    init {
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
        for (tracker in _trackers.values) {
            tracker.destroy()
        }
        _trackers.clear()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(k__setDispatchInterval) { message ->
            val seconds = Integer.valueOf(message)
            setLocalDispatchInterval(seconds)
            ""
        }
        _bridge.registerHandler(k__setDryRun) { message ->
            setDryRun(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__setOptOut) { message ->
            setAppOptOut(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__setTrackUncaughtException) { message ->
            setExceptionReportingEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(k__dispatch) {
            dispatchLocalHits()
            ""
        }
        _bridge.registerHandler(k__createTracker) { message ->
            Utils.toString(createTracker(message))
        }
        _bridge.registerHandler(k__destroyTracker) { message ->
            Utils.toString(destroyTracker(message))
        }
        _bridge.registerHandler(k__testTrackEvent) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackEvent(params))
        }
        _bridge.registerHandler(k__testTrackException) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackException(params))
        }
        _bridge.registerHandler(k__testTrackScreenView) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackScreenView(params))
        }
        _bridge.registerHandler(k__testTrackSocial) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackSocial(params))
        }
        _bridge.registerHandler(k__testTrackTiming) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackTiming(params))
        }
        _bridge.registerHandler(k__testCustomDimensionAndMetric) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testCustomDimensionAndMetric(params))
        }
        _bridge.registerHandler(k__testTrackEcommerceAction) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackEcommerceAction(params))
        }
        _bridge.registerHandler(k__testTrackEcommerceImpression) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackEcommerceImpression(params))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__setDispatchInterval)
        _bridge.deregisterHandler(k__setDryRun)
        _bridge.deregisterHandler(k__setOptOut)
        _bridge.deregisterHandler(k__setTrackUncaughtException)
        _bridge.deregisterHandler(k__dispatch)
        _bridge.deregisterHandler(k__createTracker)
        _bridge.deregisterHandler(k__destroyTracker)
        _bridge.deregisterHandler(k__testTrackEvent)
        _bridge.deregisterHandler(k__testTrackException)
        _bridge.deregisterHandler(k__testTrackScreenView)
        _bridge.deregisterHandler(k__testTrackSocial)
        _bridge.deregisterHandler(k__testTrackTiming)
        _bridge.deregisterHandler(k__testCustomDimensionAndMetric)
        _bridge.deregisterHandler(k__testTrackEcommerceAction)
        _bridge.deregisterHandler(k__testTrackEcommerceImpression)
    }

    @AnyThread
    private fun setLocalDispatchInterval(interval: Int) {
        Thread.runOnMainThread(Runnable {
            _analytics.setLocalDispatchPeriod(interval)
        })
    }

    @AnyThread
    private fun setDryRun(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            _analytics.setDryRun(enabled)
        })
    }

    @AnyThread
    private fun setAppOptOut(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            _analytics.appOptOut = enabled
        })
    }

    @AnyThread
    private fun setExceptionReportingEnabled(enabled: Boolean) {
        Thread.runOnMainThread(Runnable {
            _exceptionReportingEnabled = enabled
            for (tracker in _trackers.values) {
                tracker.setExceptionReportingEnabled(enabled)
            }
        })
    }

    @AnyThread
    private fun dispatchLocalHits() {
        Thread.runOnMainThread(Runnable {
            _analytics.dispatchLocalHits()
        })
    }

    @AnyThread
    fun createTracker(trackingId: String): Boolean {
        if (_trackers.containsKey(trackingId)) {
            return false
        }
        val tracker = GoogleAnalyticsTracker(_bridge, _analytics, trackingId)
        tracker.setExceptionReportingEnabled(_exceptionReportingEnabled)
        _trackers[trackingId] = tracker
        return true
    }

    @AnyThread
    private fun destroyTracker(trackingId: String): Boolean {
        val tracker = _trackers[trackingId] ?: return false
        tracker.destroy()
        _trackers.remove(trackingId)
        return true
    }

    @AnyThread
    fun getTracker(trackingId: String): GoogleAnalyticsTracker? {
        return _trackers[trackingId]
    }

    @AnyThread
    private fun checkDictionary(builtDict: Map<String, String>,
                                expectedDict: Map<String, String>): Boolean {
        if (builtDict.size != expectedDict.size) {
            _logger.error("Dictionary size mismatched: expected ${expectedDict.size} found ${builtDict.size}")
            return false
        }
        val allKeys = expectedDict.keys
        var matched = true
        for (key in allKeys) {
            val expectedValue = expectedDict[key]
            val value = builtDict[key]
            if (expectedValue != value) {
                _logger.error("Element value mismatched: expected $expectedValue found $value")
                matched = false
            }
        }
        return matched
    }

    @AnyThread
    private fun testTrackEvent(dict: Map<String, String>): Boolean {
        val expectedDict = EventBuilder()
            .setCategory("category")
            .setAction("action")
            .setLabel("label")
            .setValue(1)
            .build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testTrackException(dict: Map<String, String>): Boolean {
        val expectedDict = ExceptionBuilder().setDescription("description").setFatal(true).build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testTrackScreenView(dict: Map<String, String>): Boolean {
        val expectedDict = ScreenViewBuilder().build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testTrackSocial(dict: Map<String, String>): Boolean {
        val expectedDict = SocialBuilder()
            .setNetwork("network")
            .setAction("action")
            .setTarget("target")
            .build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testTrackTiming(dict: Map<String, String>): Boolean {
        val expectedDict = TimingBuilder()
            .setCategory("category")
            .setValue(1)
            .setVariable("variable")
            .setLabel("label")
            .build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testCustomDimensionAndMetric(dict: Map<String, String>): Boolean {
        val expectedDict = ScreenViewBuilder()
            .setCustomMetric(1, 1f)
            .setCustomMetric(2, 2f)
            .setCustomMetric(5, 5.5f)
            .setCustomDimension(1, "dimension_1")
            .setCustomDimension(2, "dimension_2")
            .build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testTrackEcommerceAction(dict: Map<String, String>): Boolean {
        val product0 = Product().setCategory("category0").setId("id0").setName("name0").setPrice(1.5)
        val product1 = Product().setCategory("category1").setId("id1").setName("name1").setPrice(2.0)
        val action = ProductAction(ProductAction.ACTION_PURCHASE)
            .setProductActionList("actionList")
            .setProductListSource("listSource")
            .setTransactionId("transactionId")
            .setTransactionRevenue(2.0)
        val expectedDict = ScreenViewBuilder()
            .addProduct(product0)
            .addProduct(product1)
            .setProductAction(action)
            .build()
        return checkDictionary(dict, expectedDict)
    }

    @AnyThread
    private fun testTrackEcommerceImpression(dict: Map<String, String>): Boolean {
        val product0 = Product().setCategory("category0").setId("id0").setName("name0").setPrice(1.5)
        val product1 = Product().setCategory("category1").setId("id1").setName("name1").setPrice(2.5)
        val product2 = Product().setCategory("category2").setId("id2").setName("name2").setPrice(3.0)
        val product3 = Product().setCategory("category3").setId("id3").setName("name3").setPrice(4.0)
        val expectedDict = ScreenViewBuilder()
            .addImpression(product0, "impressionList0")
            .addImpression(product1, "impressionList0")
            .addImpression(product2, "impressionList1")
            .addImpression(product3, "impressionList1")
            .build()
        return checkDictionary(dict, expectedDict)
    }
}