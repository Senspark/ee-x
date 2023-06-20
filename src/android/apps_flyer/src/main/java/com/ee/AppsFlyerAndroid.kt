package com.ee

import android.app.Activity
import android.util.Log
import com.appsflyer.AppsFlyerLib
import com.appsflyer.adrevenue.AppsFlyerAdRevenue
import com.appsflyer.adrevenue.adnetworks.generic.MediationNetwork
import com.appsflyer.adrevenue.adnetworks.generic.Scheme
import com.appsflyer.attribution.AppsFlyerRequestListener
import java.util.*
import kotlin.collections.HashMap

class AppsFlyerAndroid(private val activity: Activity) {
//    private val _af: AppsFlyerLib;
    private var _initialized = false;

    init {
        val onInitResponse = object : AppsFlyerRequestListener {
            override fun onSuccess() {
                log("Init success");
                val afRevenueBuilder = AppsFlyerAdRevenue.Builder(activity.application);
                AppsFlyerAdRevenue.initialize(afRevenueBuilder.build());
                _initialized = true;
            }

            override fun onError(errCode: Int, errDesc: String) {
                log("Init failed code=$errCode msg=$errDesc");
                _initialized = false;
            }
        };

//        _af = AppsFlyerLib.getInstance();
//        _af.setDebugLog(true);
//        _af.start(activity, "", onInitResponse);
        onInitResponse.onSuccess();
    }

    fun logAdRevenue(revenueData: AdRevenueData) {
//        if (!_initialized) {
//            return;
//        }
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