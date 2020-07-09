package com.ee.internal

import com.android.billingclient.api.BillingClient

data class ProductDefinition(
    val id: String,
    val storeSpecificId: String,
    @BillingClient.SkuType val type: ProductType
)