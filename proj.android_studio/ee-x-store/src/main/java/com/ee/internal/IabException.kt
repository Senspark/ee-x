package com.ee.internal

class IabException constructor(
    private val _result: IabResult,
    exception: Exception?) : Exception(_result.message, exception) {
    constructor(response: Int, message: String) : this(IabResult(response, message))
    constructor(response: Int, message: String, exception: Exception) : this(IabResult(response, message), exception)
    constructor(result: IabResult) : this(result, null)

    val result: IabResult
        get() = _result
}