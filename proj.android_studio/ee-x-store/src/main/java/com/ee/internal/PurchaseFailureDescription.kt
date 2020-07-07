package com.ee.internal

import kotlinx.serialization.Serializable

@Serializable
data class PurchaseFailureDescription(
    val productId: String,
    val reason: PurchaseFailureReason,
    val message: String,
    val storeSpecificErrorCode: String)