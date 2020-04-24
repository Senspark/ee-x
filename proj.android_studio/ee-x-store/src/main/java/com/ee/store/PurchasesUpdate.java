package com.ee.store;

import androidx.annotation.Nullable;

import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.Purchase;

import java.util.List;

final class PurchasesUpdate {
    @BillingResponseCode
    public final int code;

    @Nullable
    public final List<Purchase> purchases;

    PurchasesUpdate(@BillingResponseCode int code, @Nullable List<Purchase> purchases) {
        this.code = code;
        this.purchases = purchases;
    }
}
