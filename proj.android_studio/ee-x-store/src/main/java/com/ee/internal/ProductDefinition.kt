package com.ee.internal

import kotlinx.serialization.Serializable

@Serializable
data class ProductDefinition(
    val id: String,
    val storeSpecificId: String,
    val type: ProductType
)