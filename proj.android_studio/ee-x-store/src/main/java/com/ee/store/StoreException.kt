package com.ee.store

import com.android.billingclient.api.BillingClient.BillingResponseCode

internal class StoreException constructor(
    @field:BillingResponseCode @param:BillingResponseCode val responseCode: Int)
    : RuntimeException("Error during store operation. Response code: $responseCode")