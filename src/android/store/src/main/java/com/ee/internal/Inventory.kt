package com.ee.internal

import com.android.billingclient.api.BillingClient.SkuType
import com.android.billingclient.api.Purchase
import com.android.billingclient.api.PurchaseHistoryRecord
import com.android.billingclient.api.SkuDetails

class Inventory {
    private val _skuMap: MutableMap<String, SkuDetails> = HashMap()
    private val _purchaseMap: MutableMap<String, Pair<String, Purchase>> = HashMap()
    private val _consumablePurchaseHistoryMap: MutableMap<String, PurchaseHistoryRecord> = HashMap()
    private val _purchaseHistoryOfSub: MutableSet<String> = HashSet()

    val skuMap: Map<String, SkuDetails>
        get() = _skuMap

    fun hasPurchaseHistory(productId: String): Boolean {
        return _purchaseHistoryOfSub.contains(productId)
    }

    fun getSubscriptionWithHistory(): List<String> {
        return _purchaseHistoryOfSub.toList()
    }

    fun getSkuDetails(sku: String): SkuDetails? {
        return _skuMap[sku]
    }

    fun getPurchase(sku: String): Purchase? {
        return _purchaseMap[sku]?.second
    }

    fun getHistoryPurchase(sku: String): PurchaseHistoryRecord? {
        return _consumablePurchaseHistoryMap[sku]
    }

    fun hasPurchase(sku: String): Boolean {
        return _purchaseMap.containsKey(sku)
    }

    fun hasConsumablePurchaseHistory(sku: String): Boolean {
        return _consumablePurchaseHistoryMap.containsKey(sku)
    }

    fun hasDetails(sku: String): Boolean {
        return _skuMap.containsKey(sku)
    }

    fun erasePurchase(sku: String) {
        _purchaseMap.remove(sku)
    }

    fun getAllOwnedSkus(): List<String> {
        return _purchaseMap.keys.toList()
    }

    fun getAllSkus(@SkuType itemType: String): List<String> {
        return _skuMap.values
            .filter { it.type == itemType }
            .map { it.sku }
    }

    fun getAllOwnedSkus(@SkuType itemType: String): List<String> {
        return _purchaseMap.values
            .filter { it.first == itemType }
            .map { it.second.skus.get(0) }
    }

    fun getAllPurchases(): List<Pair<String, Purchase>> {
        return _purchaseMap.values.toList()
    }

    fun addSkuDetails(details: SkuDetails) {
        _skuMap[details.sku] = details
    }

    fun addPurchase(@SkuType itemType: String, purchase: Purchase) {
        _purchaseMap[purchase.skus.get(0)] = Pair(itemType, purchase)
    }

    fun addPurchaseToSubscriptionPurchaseHistory(sku: String) {
        _purchaseHistoryOfSub.add(sku)
    }

    fun addPurchaseToConsumablePurchaseHistory(sku: String, record: PurchaseHistoryRecord) {
        _consumablePurchaseHistoryMap[sku] = record
    }
}