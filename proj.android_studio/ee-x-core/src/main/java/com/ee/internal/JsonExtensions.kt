package com.ee.internal

import androidx.annotation.AnyThread
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import kotlinx.serialization.builtins.ListSerializer
import kotlinx.serialization.builtins.MapSerializer
import kotlinx.serialization.json.Json
import kotlinx.serialization.serializer

@AnyThread
@ImplicitReflectionSerializer
@UnstableDefault
inline fun <reified T : Any> deserialize(string: String): T {
    return Json.parse(T::class.serializer(), string)
}

@AnyThread
@ImplicitReflectionSerializer
@UnstableDefault
inline fun <reified T : Any> deserializeList(string: String): List<T> {
    val valueSerializer = T::class.serializer()
    val serializer = ListSerializer(valueSerializer)
    return Json.parse(serializer, string)
}

@AnyThread
@ImplicitReflectionSerializer
@UnstableDefault
inline fun <reified K : Any, reified V : Any> deserializeMap(string: String): Map<K, V> {
    val keySerializer = K::class.serializer()
    val valueSerializer = V::class.serializer()
    val serializer = MapSerializer(keySerializer, valueSerializer)
    return Json.parse(serializer, string)
}

@AnyThread
@ImplicitReflectionSerializer
@UnstableDefault
inline fun <reified T : Any> T.serialize(): String {
    return Json.stringify(T::class.serializer(), this)
}