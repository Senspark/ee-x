package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.ee.internal.GoogleAnalyticsTracker
import com.ee.internal.deserialize
import com.google.android.gms.analytics.GoogleAnalytics
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
class GoogleAnalyticsBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(GoogleAnalyticsBridge::class.java.name)

        private const val kPrefix = "GoogleAnalyticsBridge"
        private const val kSetDispatchInterval = "${kPrefix}SetDispatchInterval"
        private const val kSetDryRun = "${kPrefix}SetDryRun"
        private const val kSetOptOut = "${kPrefix}SetOptOut"
        private const val kSetTrackUncaughtException = "${kPrefix}SetTrackUncaughtException"
        private const val kDispatch = "${kPrefix}Dispatch"
        private const val kCreateTracker = "${kPrefix}CreateTracker"
        private const val kDestroyTracker = "${kPrefix}DestroyTracker"
        private const val kTestTrackEvent = "${kPrefix}TestTrackEvent"
        private const val kTestTrackException = "${kPrefix}TestTrackException"
        private const val kTestTrackScreenView = "${kPrefix}TestTrackScreenView"
        private const val kTestTrackSocial = "${kPrefix}TestTrackSocial"
        private const val kTestTrackTiming = "${kPrefix}TestTrackTiming"
        private const val kTestCustomDimensionAndMetric = "${kPrefix}TestCustomDimensionAndMetric"
        private const val kTestTrackEcommerceAction = "${kPrefix}TestTrackEcommerceAction"
        private const val kTestTrackEcommerceImpression = "${kPrefix}TestTrackEcommerceImpression"
    }

    private val _analytics = GoogleAnalytics.getInstance(_context)
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
        _bridge.registerHandler(kSetDispatchInterval) { message ->
            val seconds = Integer.valueOf(message)
            setLocalDispatchInterval(seconds)
            ""
        }
        _bridge.registerHandler(kSetDryRun) { message ->
            setDryRun(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetOptOut) { message ->
            setAppOptOut(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetTrackUncaughtException) { message ->
            setExceptionReportingEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kDispatch) {
            dispatchLocalHits()
            ""
        }
        _bridge.registerHandler(kCreateTracker) { message ->
            Utils.toString(createTracker(message))
        }
        _bridge.registerHandler(kDestroyTracker) { message ->
            Utils.toString(destroyTracker(message))
        }
        _bridge.registerHandler(kTestTrackEvent) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackEvent(params))
        }
        _bridge.registerHandler(kTestTrackException) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackException(params))
        }
        _bridge.registerHandler(kTestTrackScreenView) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackScreenView(params))
        }
        _bridge.registerHandler(kTestTrackSocial) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackSocial(params))
        }
        _bridge.registerHandler(kTestTrackTiming) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackTiming(params))
        }
        _bridge.registerHandler(kTestCustomDimensionAndMetric) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testCustomDimensionAndMetric(params))
        }
        _bridge.registerHandler(kTestTrackEcommerceAction) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackEcommerceAction(params))
        }
        _bridge.registerHandler(kTestTrackEcommerceImpression) { message ->
            val params = deserialize<Map<String, String>>(message)
            Utils.toString(testTrackEcommerceImpression(params))
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kSetDispatchInterval)
        _bridge.deregisterHandler(kSetDryRun)
        _bridge.deregisterHandler(kSetOptOut)
        _bridge.deregisterHandler(kSetTrackUncaughtException)
        _bridge.deregisterHandler(kDispatch)
        _bridge.deregisterHandler(kCreateTracker)
        _bridge.deregisterHandler(kDestroyTracker)
        _bridge.deregisterHandler(kTestTrackEvent)
        _bridge.deregisterHandler(kTestTrackException)
        _bridge.deregisterHandler(kTestTrackScreenView)
        _bridge.deregisterHandler(kTestTrackSocial)
        _bridge.deregisterHandler(kTestTrackTiming)
        _bridge.deregisterHandler(kTestCustomDimensionAndMetric)
        _bridge.deregisterHandler(kTestTrackEcommerceAction)
        _bridge.deregisterHandler(kTestTrackEcommerceImpression)
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