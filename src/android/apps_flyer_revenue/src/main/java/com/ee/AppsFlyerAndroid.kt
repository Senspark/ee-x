package com.ee

import android.app.Activity
import android.util.Log
import com.appsflyer.adrevenue.AppsFlyerAdRevenue
import com.appsflyer.adrevenue.adnetworks.generic.MediationNetwork
import com.appsflyer.adrevenue.adnetworks.generic.Scheme
import com.appsflyer.api.PurchaseClient
import com.appsflyer.api.Store
import com.appsflyer.internal.models.InAppPurchaseValidationResult
import com.appsflyer.internal.models.SubscriptionValidationResult
import java.util.*
import kotlin.collections.HashMap

class AppsFlyerAndroid(private val activity: Activity) {
    init {
        // af_ad_revenue
        val application = activity.application;
        val afRevenueBuilder = AppsFlyerAdRevenue.Builder(application);
        AppsFlyerAdRevenue.initialize(afRevenueBuilder.build());

        // af_purchase
        val builder = PurchaseClient.Builder(application, Store.GOOGLE)
            .logSubscriptions(true)
            .autoLogInApps(true)
            .setSandbox(false)
            .setInAppValidationResultListener(AppsFlyerIapResultListener())
            .setSubscriptionValidationResultListener(AppsFlyerSubscriptionResultListener())

        val afPurchaseClient = builder.build();
        afPurchaseClient.startObservingTransactions();

        log("Init success");
    }

    fun logAdRevenue(revenueData: AdRevenueData) {
        val customParams: MutableMap<String, String> = HashMap()
        customParams[Scheme.AD_UNIT] = revenueData.adUnitId
        customParams[Scheme.AD_TYPE] = revenueData.adFormat
        customParams[Scheme.PLACEMENT] = "place"
        customParams[Scheme.ECPM_PAYLOAD] = "encrypt"

        var mediationNetwork = MediationNetwork.googleadmob;
        when (revenueData.mediationName) {
            "applovin" -> MediationNetwork.applovinmax;
        }

        AppsFlyerAdRevenue.logAdRevenue(
            revenueData.networkName,
            mediationNetwork,
            Currency.getInstance(Locale.US),
            revenueData.revenue,
            customParams
        );
    }

    private fun log(message: String) {
        Log.d("AppsFlyer", message);
    }
}

class AppsFlyerIapResultListener : PurchaseClient.InAppPurchaseValidationResultListener {
    override fun onResponse(result: Map<String, InAppPurchaseValidationResult>?) {
        result?.forEach { (k: String, v: InAppPurchaseValidationResult?) ->
            if (v.success && v.productPurchase != null) {
                val productPurchase = v.productPurchase!!
                val orderId = productPurchase.orderId
                val isTest = productPurchase.purchaseType != null && productPurchase.purchaseType == 0
                val productId = productPurchase.productId
                
                log("Validation success: $k $orderId $isTest $productId")
                log("Product info $productPurchase")
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
        Log.d("AppsFlyer", message);
    }
}

class AppsFlyerSubscriptionResultListener : PurchaseClient.SubscriptionPurchaseValidationResultListener {
    override fun onResponse(result: Map<String, SubscriptionValidationResult>?) {
        result?.forEach { (k: String, v: SubscriptionValidationResult?) ->
            if (v.success) {
                val productPurchase = v.subscriptionPurchase;
                log("Validation success: $k $productPurchase")
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
        Log.d("AppsFlyer", message);
    }
}