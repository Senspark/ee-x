package com.ee.internal

import kotlinx.serialization.Serializable

@Serializable
data class ProductDescription(
    val storeSpecificId: String,
    val metadata: ProductMetadata,
    val receipt: String,
    val transactionId: String
)