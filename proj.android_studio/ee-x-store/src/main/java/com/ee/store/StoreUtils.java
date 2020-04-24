package com.ee.store;

import androidx.annotation.NonNull;

import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchaseHistoryRecord;
import com.android.billingclient.api.SkuDetails;

import java.util.HashMap;
import java.util.Map;

class StoreUtils {
    @NonNull
    static Map<String, Object> convertSkuDetailsToDictionary(@NonNull SkuDetails item) {
        Map<String, Object> dict = new HashMap<>();
        dict.put("description", item.getDescription());
        dict.put("free_trial_period", item.getFreeTrialPeriod());
        dict.put("price", item.getPrice());
        dict.put("price_amount_micros", item.getPriceAmountMicros());
        dict.put("price_currency_code", item.getPriceCurrencyCode());
        dict.put("sku", item.getSku());
        dict.put("subscription_period", item.getSubscriptionPeriod());
        dict.put("title", item.getTitle());
        return dict;
    }

    @NonNull
    static Map<String, Object> convertPurchaseToDictionary(@NonNull Purchase item) {
        Map<String, Object> dict = new HashMap<>();
        dict.put("package_name", item.getPackageName());
        dict.put("purchase_state", item.getPurchaseState());
        dict.put("purchase_time", item.getPurchaseTime());
        dict.put("purchase_token", item.getPurchaseToken());
        dict.put("signature", item.getSignature());
        dict.put("sku", item.getSku());
        dict.put("is_acknowledged", item.isAcknowledged());
        dict.put("is_auto_renewing", item.isAutoRenewing());
        return dict;
    }

    @NonNull
    static Map<String, Object> convertRecordToDictionary(@NonNull PurchaseHistoryRecord item) {
        Map<String, Object> dict = new HashMap<>();
        dict.put("purchase_time", item.getPurchaseTime());
        dict.put("purchase_token", item.getPurchaseToken());
        dict.put("signature", item.getSignature());
        dict.put("sku", item.getSku());
        return dict;
    }
}
