package com.ee.internal

import androidx.annotation.AnyThread
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.builtins.ListSerializer
import kotlinx.serialization.builtins.MapSerializer
import kotlinx.serialization.json.Json
import kotlinx.serialization.serializer

@AnyThread
@InternalSerializationApi
inline fun <reified T : Any> deserialize(string: String): T {
    return Json.decodeFromString(T::class.serializer(), string)
}

@AnyThread
@InternalSerializationApi
inline fun <reified T : Any> deserializeList(string: String): List<T> {
    val valueSerializer = T::class.serializer()
    val serializer = ListSerializer(valueSerializer)
    return Json.decodeFromString(serializer, string)
}

@AnyThread
@InternalSerializationApi
inline fun <reified K : Any, reified V : Any> deserializeMap(string: String): Map<K, V> {
    val keySerializer = K::class.serializer()
    val valueSerializer = V::class.serializer()
    val serializer = MapSerializer(keySerializer, valueSerializer)
    return Json.decodeFromString(serializer, string)
}

@AnyThread
@InternalSerializationApi
inline fun <reified T : Any> T.serialize(): String {
    return Json.encodeToString(T::class.serializer(), this)
}

@AnyThread
@InternalSerializationApi
inline fun <reified T : Any> List<T>.serialize(): String {
    val valueSerializer = T::class.serializer()
    val serializer = ListSerializer(valueSerializer)
    return Json.encodeToString(serializer, this)
}

@AnyThread
@InternalSerializationApi
inline fun <reified K : Any, reified V : Any> Map<K, V>.serialize(): String {
    val keySerializer = K::class.serializer()
    val valueSerializer = V::class.serializer()
    val serializer = MapSerializer(keySerializer, valueSerializer)
    return Json.encodeToString(serializer, this)
}