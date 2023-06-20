package com.ee

import android.app.Activity
import android.util.Log
import com.appsflyer.adrevenue.AppsFlyerAdRevenue
import com.appsflyer.adrevenue.adnetworks.generic.MediationNetwork
import com.appsflyer.adrevenue.adnetworks.generic.Scheme
import java.util.*
import kotlin.collections.HashMap

class AppsFlyerAndroid(private val activity: Activity) {
    init {
        log("Init success");
        val afRevenueBuilder = AppsFlyerAdRevenue.Builder(activity.application);
        AppsFlyerAdRevenue.initialize(afRevenueBuilder.build());
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