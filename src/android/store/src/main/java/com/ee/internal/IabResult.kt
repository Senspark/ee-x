package com.ee.internal

private fun getResponseDesc(code: Int): String {
    val iabMsgs = "0:OK/1:User Canceled/2:Unknown/3:Billing Unavailable/4:Item unavailable/5:Developer Error/6:Error/7:Item Already Owned/8:Item not owned".split("/").toList()
    val iabhelperMsgs = "0:OK/-1001:Remote exception during initialization/-1002:Bad response received/-1003:Purchase signature verification failed/-1004:Send intent failed/-1005:User cancelled/-1006:Unknown purchase response/-1007:Missing token/-1008:Unknown error/-1009:Subscriptions not available/-1010:Invalid consumption attempt".split("/")
    return if (code <= -1000) {
        val index = -1000 - code
        if (index >= 0 && index < iabhelperMsgs.size) iabhelperMsgs[index] else "$code:Unknown IAB Helper Error"
    } else {
        if (code >= 0 && code < iabMsgs.size) iabMsgs[code] else "$code:Unknown"
    }
}

class IabResult(
    private val _response: Int,
    message: String?
) {
    private val _message: String = if (message != null && message.trim().isNotEmpty()) {
        "$message (response: ${getResponseDesc(_response)})"
    } else {
        getResponseDesc(_response)
    }

    val response: Int
        get() = _response

    val message: String
        get() = _message

    val isSuccess: Boolean
        get() = _response == 0

    val isFailure: Boolean
        get() = !isSuccess
}