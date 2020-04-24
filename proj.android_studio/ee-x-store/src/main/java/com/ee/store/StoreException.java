package com.ee.store;

import com.android.billingclient.api.BillingClient.BillingResponseCode;

public final class StoreException extends RuntimeException {
    @BillingResponseCode
    public final int responseCode;

    StoreException(@BillingResponseCode int responseCode) {
        super("Error during store operation. Response code: " + responseCode);
        this.responseCode = responseCode;
    }
}
