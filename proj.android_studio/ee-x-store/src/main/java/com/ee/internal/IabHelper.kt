package com.ee.internal

import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.BillingClient.SkuType
import com.ee.IStoreBridge

class IabHelper(private val _bridge: IStoreBridge) {
    private var _setupDone = false
    private val _inventory = Inventory()
    private var _subscriptionsSupported = false
    private var _subscriptionPurchaseHistorySupported = false

    val setupDone: Boolean
        get() = _setupDone

    suspend fun startSetup(): IabResult {
        if (_setupDone) {
            throw IllegalStateException("IAB helper is already setup")
        }
        return finishSetup()
    }

    private suspend fun finishSetup(): IabResult {
        if (_bridge.isFeatureSupported("inapp")) {
            _subscriptionsSupported = false
            _subscriptionPurchaseHistorySupported = false
            return IabResult(BillingClient.BillingResponseCode.FEATURE_NOT_SUPPORTED, "Billing V3 not supported")
        }
        if (_bridge.isFeatureSupported(BillingClient.FeatureType.SUBSCRIPTIONS)) {
            _subscriptionsSupported = true
            _subscriptionPurchaseHistorySupported = true
        }
        _setupDone = true
        return IabResult(BillingClient.BillingResponseCode.OK, "Setup successful")
    }

    val subscriptionsSupported: Boolean
        get() = _subscriptionsSupported

    val subscriptionPurchaseHistorySupported: Boolean
        get() = _subscriptionPurchaseHistorySupported

    suspend fun queryInventory(querySkuDetails: Boolean,
                               moreSkus: List<String>): Inventory {
        var response = queryPurchases(_inventory, SkuType.INAPP)
        if (response != BillingClient.BillingResponseCode.OK) {
            throw IabException(response, "Error refreshing inventory (querying owned items)")
        }
        if (querySkuDetails) {
            response = querySkuDetails(SkuType.INAPP, _inventory, moreSkus)
            if (response != BillingClient.BillingResponseCode.OK) {
                throw IabException(response, "Error refreshing inventory (querying prices of items)")
            }
        }
        if (_subscriptionsSupported) {
            response = queryPurchases(_inventory, SkuType.SUBS)
            if (response != BillingClient.BillingResponseCode.OK) {
                throw IabException(response, "Error refreshing inventory (querying owned subscriptions)")
            }
            if (querySkuDetails) {
                response = querySkuDetails(SkuType.SUBS, _inventory, moreSkus)
                if (response != BillingClient.BillingResponseCode.OK) {
                    throw IabException(response, "Error refreshing inventory (querying prices of subscriptions)")
                }
            }
        }
        if (_subscriptionPurchaseHistorySupported) {
            queryPurchaseHistory(_inventory, SkuType.SUBS)
            response = queryPurchaseHistory(_inventory, SkuType.INAPP)
            if (response != BillingClient.BillingResponseCode.OK) {
                throw IabException(response, "Error query Purchase History")
            }
        }
        return _inventory
    }

    suspend fun consume(@SkuType itemType: String, token: String, sku: String) {
        if (itemType != SkuType.INAPP) {
            throw IabException(-1010, "Items of type '$itemType' can't be consumed")
        }
        if (token.isNotEmpty()) {
            try {
                _bridge.consume(token)
            } catch (ex: StoreException) {
                throw IabException(ex.responseCode, "Error consuming sku $sku")
            }
        } else {
            throw IabException(-1007, "PurchaseInfo is missing token for sku $sku")
        }
    }

    suspend fun acknowledge(token: String, sku: String) {
        if (token.isNotEmpty()) {
            try {
                _bridge.acknowledge(token)
            } catch (ex: StoreException) {
                throw IabException(ex.responseCode, "Error acknowledging sku $sku")
            }
        } else {
            throw IabException(-1007, "PurchaseInfo is missing token for sku $sku")
        }
    }

    private suspend fun queryPurchaseHistory(inv: Inventory, @SkuType itemType: String): Int {
        var responseCode = BillingClient.BillingResponseCode.OK
        try {
            val recordList = _bridge.getPurchaseHistory(itemType)
            if (itemType == SkuType.INAPP) {
                recordList.forEach {
                    inv.addPurchaseToConsumablePurchaseHistory(it.sku, it)
                }
            } else {
                recordList.forEach {
                    inv.addPurchaseToSubscriptionPurchaseHistory(it.sku)
                }
            }
        } catch (ex: StoreException) {
            responseCode = ex.responseCode
        }
        return responseCode
    }

    @BillingClient.BillingResponseCode
    suspend fun queryPurchases(inv: Inventory, @SkuType itemType: String): Int {
        var responseCode = BillingClient.BillingResponseCode.OK
        try {
            val purchaseList = _bridge.getPurchases(itemType)
            purchaseList.forEach {
                inv.addPurchase(itemType, it)
            }
        } catch (ex: StoreException) {
            responseCode = ex.responseCode
        }
        return responseCode
    }

    @BillingClient.BillingResponseCode
    private suspend fun querySkuDetails(@SkuType itemType: String,
                                        inv: Inventory,
                                        moreSkus: List<String>): Int {
        val skuList = inv.getAllOwnedSkus(itemType).plus(moreSkus)
        if (skuList.isEmpty()) {
            return BillingClient.BillingResponseCode.OK
        }
        var responseCode = BillingClient.BillingResponseCode.OK
        try {
            val detailsList = _bridge.getSkuDetails(itemType, skuList)
            detailsList.forEach {
                inv.addSkuDetails(it)
            }
        } catch (ex: StoreException) {
            responseCode = ex.responseCode
        }
        return responseCode
    }
}