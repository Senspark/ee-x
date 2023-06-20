package com.ee.internal

import com.google.android.gms.ads.AdapterResponseInfo

@Suppress("unused")
class AdPaidResponse(
    val adUnitId: String,
    val adFormat: String,
    val valueMicros: Long,
    val info: AdapterResponseInfo?
)