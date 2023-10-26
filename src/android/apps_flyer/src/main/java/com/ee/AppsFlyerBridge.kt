package com.ee

import android.app.Activity
import android.app.Application
import androidx.annotation.AnyThread
import com.appsflyer.AppsFlyerConversionListener
import com.appsflyer.AppsFlyerLib
import com.appsflyer.adrevenue.AppsFlyerAdRevenue
import com.appsflyer.adrevenue.adnetworks.generic.MediationNetwork
import com.appsflyer.adrevenue.adnetworks.generic.Scheme
import com.appsflyer.api.PurchaseClient
import com.appsflyer.api.Store
import com.appsflyer.internal.models.InAppPurchaseValidationResult
import com.appsflyer.internal.models.SubscriptionValidationResult
import com.ee.internal.deserialize
import com.ee.internal.serialize
import kotlinx.serialization.Serializable
import kotlinx.serialization.json.JsonPrimitive
import kotlinx.serialization.json.doubleOrNull
import kotlinx.serialization.json.longOrNull
import java.util.Currency
import java.util.Locale

class AppsFlyerBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?
) : IPlugin {
    companion object {
        private val kTag = AppsFlyerBridge::class.java.name
        private const val kPrefix = "AppsFlyerBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kStartTracking = "${kPrefix}StartTracking"
        private const val kGetDeviceId = "${kPrefix}GetDeviceId"
        private const val kSetDebugEnabled = "${kPrefix}SetDebugEnabled"
        private const val kSetStopTracking = "${kPrefix}SetStopTracking"
        private const val kTrackEvent = "${kPrefix}TrackEvent"
        private const val kLogAdRevenue = "${kPrefix}LogAdRevenue"
        private const val kOnPurchaseValidated = "${kPrefix}OnPurchaseValidated"
    }

    private val _tracker = AppsFlyerLib.getInstance()

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
    }

    @Serializable
    private class InitializeRequest(
        val devKey: String,
        val iosAppId: String
    )

    @Serializable
    private class TrackEventRequest(
        val name: String,
        val values: Map<String, JsonPrimitive>
    )

    @Serializable
    @Suppress("unused")
    class AdRevenueData(
        val mediationNetwork: String,
        val monetizationNetwork: String,
        val currencyCode: String,
        val adFormat: String,
        val adUnit: String,
        val revenue: Double,
    )

    @Serializable
    @Suppress("unused")
    class PurchaseValidateData(
        val isSuccess: Boolean,
        val isTestPurchase: Boolean,
        val orderId: String,
        val productId: String,
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) { message ->
            val request = deserialize<InitializeRequest>(message)
            initialize(request.devKey)
            ""
        }
        _bridge.registerHandler(kStartTracking) {
            startTracking()
            ""
        }
        _bridge.registerHandler(kGetDeviceId) {
            deviceId
        }
        _bridge.registerHandler(kSetDebugEnabled) { message ->
            setDebugEnabled(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kSetStopTracking) { message ->
            setStopTracking(Utils.toBoolean(message))
            ""
        }
        _bridge.registerHandler(kTrackEvent) { message ->
            val request = deserialize<TrackEventRequest>(message)
            trackEvent(request.name, request.values)
            ""
        }
        _bridge.registerHandler(kLogAdRevenue) { message ->
            val request = deserialize<AdRevenueData>(message)
            logAdRevenue(request)
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
        _bridge.deregisterHandler(kStartTracking)
        _bridge.deregisterHandler(kGetDeviceId)
        _bridge.deregisterHandler(kSetDebugEnabled)
        _bridge.deregisterHandler(kSetStopTracking)
        _bridge.deregisterHandler(kTrackEvent)
    }

    @AnyThread
    fun initialize(devKey: String) {
        Thread.runOnMainThread {
            val listener = object : AppsFlyerConversionListener {
                override fun onConversionDataSuccess(conversionData: Map<String, Any>) {
                    for (key in conversionData.keys) {
                        _logger.debug("$kTag: ${this::onConversionDataSuccess.name}: $key = ${conversionData[key]}")
                    }
                }

                override fun onConversionDataFail(errorMessage: String) {
                    _logger.debug("$kTag: ${this::onConversionDataFail.name}: $errorMessage")
                }

                override fun onAppOpenAttribution(conversionData: Map<String, String>) {
                    for (key in conversionData.keys) {
                        _logger.debug("$kTag: ${this::onAppOpenAttribution.name}: $key = ${conversionData[key]}")
                    }
                }

                override fun onAttributionFailure(errorMessage: String) {
                    _logger.debug("$kTag: ${this::onAttributionFailure.name}: $errorMessage")
                }
            }
            _tracker.init(devKey, listener, _application)
            _tracker.anonymizeUser(false)

            // af_ad_revenue
            val afRevenueBuilder = AppsFlyerAdRevenue.Builder(_application)
            AppsFlyerAdRevenue.initialize(afRevenueBuilder.build())

            val onValidated = { data: PurchaseValidateData ->
                val message = data.serialize()
                _bridge.callCpp(kOnPurchaseValidated, message)
            }

            // af_purchase
            val builder = PurchaseClient.Builder(_application, Store.GOOGLE)
                .logSubscriptions(true)
                .autoLogInApps(true)
                .setSandbox(false)
                .setInAppValidationResultListener(AppsFlyerIapResultListener(_logger, onValidated))
                .setSubscriptionValidationResultListener(
                    AppsFlyerSubscriptionResultListener(
                        _logger,
                        onValidated
                    )
                )

            val afPurchaseClient = builder.build()
            afPurchaseClient.startObservingTransactions()
        }
    }

    @AnyThread
    fun startTracking() {
        Thread.runOnMainThread {
            _tracker.start(_application)
        }
    }

    val deviceId: String
        @AnyThread get() = _tracker.getAppsFlyerUID(_application)!!

    @AnyThread
    fun setDebugEnabled(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.setDebugLog(enabled)
        }
    }

    @AnyThread
    fun setStopTracking(enabled: Boolean) {
        Thread.runOnMainThread {
            _tracker.stop(enabled, _application)
        }
    }

    @AnyThread
    fun trackEvent(name: String, values: Map<String, JsonPrimitive>) {
        Thread.runOnMainThread {
            val parsedValues = HashMap<String, Any>()
            for ((key, value) in values) {
                if (value.isString) {
                    parsedValues[key] = value.content
                    continue
                }
                val long = value.longOrNull
                if (long != null) {
                    parsedValues[key] = long
                    continue
                }
                val double = value.doubleOrNull
                if (double != null) {
                    parsedValues[key] = double
                    continue
                }
                assert(false)
            }
            _tracker.logEvent(_application, name, values)
        }
    }

    private fun logAdRevenue(revenueData: AdRevenueData) {
        val customParams: MutableMap<String, String> = HashMap()
        customParams[Scheme.AD_UNIT] = revenueData.adUnit
        customParams[Scheme.AD_TYPE] = revenueData.adFormat
        customParams[Scheme.PLACEMENT] = "place"
        customParams[Scheme.ECPM_PAYLOAD] = "encrypt"

        val mediationNetwork = when (revenueData.mediationNetwork) {
            "applovin" -> MediationNetwork.applovinmax
            "admob" -> MediationNetwork.googleadmob
            else -> MediationNetwork.customMediation
        }

        _logger.info("$kTag: ${this::logAdRevenue.name}: ${revenueData.mediationNetwork} ${revenueData.revenue}")
        AppsFlyerAdRevenue.logAdRevenue(
            revenueData.monetizationNetwork,
            mediationNetwork,
            Currency.getInstance(Locale.US),
            revenueData.revenue,
            customParams
        )
    }
}


class AppsFlyerIapResultListener(
    private val _logger: ILogger,
    private val _onValidated: (AppsFlyerBridge.PurchaseValidateData) -> Unit,
) : PurchaseClient.InAppPurchaseValidationResultListener {

    companion object {
        private val kTag = AppsFlyerIapResultListener::class.java.name
    }

    override fun onResponse(result: Map<String, InAppPurchaseValidationResult>?) {
        result?.forEach { (k: String, v: InAppPurchaseValidationResult?) ->
            if (v.success && v.productPurchase != null) {
                val productPurchase = v.productPurchase!!
                val orderId = productPurchase.orderId
                val isTest =
                    productPurchase.purchaseType != null && productPurchase.purchaseType == 0
                val productId = productPurchase.productId
                val isSuccess = productPurchase.purchaseState == 0

                log("Validation success: $k $orderId $isTest $productId")
//                log("Product info $productPurchase")
                _onValidated(
                    AppsFlyerBridge.PurchaseValidateData(
                        isSuccess,
                        isTest,
                        orderId,
                        productId
                    )
                )
            } else {
                val failureData = v.failureData
                log("Validation fail: $k $failureData")
            }
        }
    }

    override fun onFailure(result: String, error: Throwable?) {
        log("Validation fail: $result, $error");
    }

    private fun log(message: String) {
        _logger.info("$kTag: $message")
    }
}

class AppsFlyerSubscriptionResultListener(
    private val _logger: ILogger,
    private val _onValidated: (AppsFlyerBridge.PurchaseValidateData) -> Unit,
) : PurchaseClient.SubscriptionPurchaseValidationResultListener {

    companion object {
        private val kTag = AppsFlyerSubscriptionResultListener::class.java.name
    }

    override fun onResponse(result: Map<String, SubscriptionValidationResult>?) {
        result?.forEach { (k: String, v: SubscriptionValidationResult?) ->
            val productPurchase = v.subscriptionPurchase
            if (v.success && productPurchase != null) {
                val isSuccess = productPurchase.subscriptionState == "SUBSCRIPTION_STATE_ACTIVE"
                val isTest = productPurchase.testPurchase != null
                val orderId = productPurchase.latestOrderId
                val productId = productPurchase.lineItems[0].productId

//                log("Validation success: $k $productPurchase")
                log("Validation success: $k $orderId $isTest $productId")

                _onValidated(
                    AppsFlyerBridge.PurchaseValidateData(
                        isSuccess,
                        isTest,
                        orderId,
                        productId
                    )
                )
            } else {
                val failureData = v.failureData
                log("Validation fail: $k $failureData")
            }
        }
    }

    override fun onFailure(result: String, error: Throwable?) {
        log("Validation fail: $result, $error")
    }

    private fun log(message: String) {
        _logger.info("${kTag}: $message")
    }
}