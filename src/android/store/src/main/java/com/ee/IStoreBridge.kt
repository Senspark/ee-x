package com.ee

import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.BillingClient.FeatureType
import com.android.billingclient.api.BillingClient.SkuType
import com.android.billingclient.api.Purchase
import com.android.billingclient.api.PurchaseHistoryRecord
import com.android.billingclient.api.SkuDetails
import io.reactivex.rxjava3.core.Completable
import io.reactivex.rxjava3.core.Single

interface IStoreBridge {
    suspend fun connect(): BillingClient
    suspend fun isFeatureSupported(@FeatureType featureType: String): Boolean
    suspend fun getSkuDetails(@SkuType skuType: String, skuList: List<String>): List<SkuDetails>
    suspend fun getPurchases(@SkuType skuType: String): List<Purchase>
    suspend fun getPurchaseHistory(@SkuType skuType: String): List<PurchaseHistoryRecord>
    suspend fun purchase(sku: String): Purchase
    suspend fun consume(purchaseToken: String)
    suspend fun acknowledge(purchaseToken: String)

    /// Legacy functions use by Java side.
    fun connectRx(): Single<BillingClient>
    fun getSkuDetailsRx(@SkuType skuType: String, skuList: List<String>): Single<List<SkuDetails>>
    fun getPurchasesRx(@SkuType skuType: String): Single<List<Purchase>>
    fun getPurchaseHistoryRx(@SkuType skuType: String): Single<List<PurchaseHistoryRecord>>
    fun purchaseRx(sku: String): Single<Purchase>
    fun consumeRx(purchaseToken: String): Completable
    fun acknowledgeRx(purchaseToken: String): Completable
}