package com.ee

internal object FirebaseEventType {
    const val Long = 0
    const val Double = 1
    const val String = 2
}

internal class FirebaseEventParameter(
    val _type: Int,
    val _value: String) {
    val type = _type
    val longValue = _value.toLong()
    val doubleValue = _value.toDouble()
    val stringValue = _value
}