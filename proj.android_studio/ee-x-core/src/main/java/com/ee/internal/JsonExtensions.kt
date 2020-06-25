package com.ee.internal

import androidx.annotation.AnyThread
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault
import kotlinx.serialization.json.Json
import kotlinx.serialization.serializer

@AnyThread
@UnstableDefault
@ImplicitReflectionSerializer
inline fun <reified T : Any> deserialize(string: String): T {
    return Json.parse(T::class.serializer(), string)
}

@AnyThread
@UnstableDefault
@ImplicitReflectionSerializer
inline fun <reified T : Any> T.serialize(): String {
    return Json.stringify(T::class.serializer(), this)
}