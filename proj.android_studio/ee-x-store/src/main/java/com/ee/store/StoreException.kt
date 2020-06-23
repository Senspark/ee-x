package com.ee.store

import com.android.billingclient.api.BillingClient.BillingResponseCode

class StoreException(
    @field:BillingResponseCode @param:BillingResponseCode val responseCode: Int)
    : RuntimeException("Error during store operation. Response code: $responseCode")