package com.ee.internal

enum class PurchaseFailureReason {
    BillingUnavailable,
    ExistingPurchasePending,
    ItemUnavailable,
    SignatureInvalid,
    UserCancelled,
    PaymentDeclined,
    DuplicateTransaction,
    Unknown,
}