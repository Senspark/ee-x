package com.ee.internal

import com.android.billingclient.api.Purchase
import com.android.billingclient.api.PurchaseHistoryRecord
import com.android.billingclient.api.SkuDetails

internal object StoreUtils {
    fun convertSkuDetailsToDictionary(item: SkuDetails): Map<String, Any> {
        val dict: MutableMap<String, Any> = HashMap()
        dict["description"] = item.description
        dict["free_trial_period"] = item.freeTrialPeriod
        dict["price"] = item.price
        dict["price_amount_micros"] = item.priceAmountMicros
        dict["price_currency_code"] = item.priceCurrencyCode
        dict["sku"] = item.sku
        dict["subscription_period"] = item.subscriptionPeriod
        dict["title"] = item.title
        return dict
    }

    fun convertPurchaseToDictionary(item: Purchase): Map<String, Any> {
        val dict: MutableMap<String, Any> = HashMap()
        dict["package_name"] = item.packageName
        dict["purchase_state"] = item.purchaseState
        dict["purchase_time"] = item.purchaseTime
        dict["purchase_token"] = item.purchaseToken
        dict["signature"] = item.signature
        dict["sku"] = item.sku
        dict["is_acknowledged"] = item.isAcknowledged
        dict["is_auto_renewing"] = item.isAutoRenewing
        return dict
    }

    fun convertRecordToDictionary(item: PurchaseHistoryRecord): Map<String, Any> {
        val dict: MutableMap<String, Any> = HashMap()
        dict["purchase_time"] = item.purchaseTime
        dict["purchase_token"] = item.purchaseToken
        dict["signature"] = item.signature
        dict["sku"] = item.sku
        return dict
    }
}