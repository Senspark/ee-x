package com.ee.store

import com.android.billingclient.api.BillingClient.BillingResponseCode
import com.android.billingclient.api.Purchase

internal class PurchasesUpdate(
    @field:BillingResponseCode @param:BillingResponseCode val code: Int,
    val purchases: List<Purchase>)