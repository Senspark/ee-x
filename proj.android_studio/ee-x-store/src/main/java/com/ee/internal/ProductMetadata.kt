package com.ee.internal

import kotlinx.serialization.Serializable

@Serializable
data class ProductMetadata(
    val localizedPriceString: String,
    val localizedTitle: String,
    val localizedDescription: String,
    val isoCurrencyCode: String,
    val localizedPrice: Long)