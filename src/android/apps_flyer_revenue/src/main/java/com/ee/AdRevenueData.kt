package com.ee

import kotlinx.serialization.Serializable

@Serializable
@Suppress("unused")
class AdRevenueData(
    val networkName: String,
    val mediationName: String,
    val adUnitId: String,
    val adFormat: String,
    val revenue: Double,
)