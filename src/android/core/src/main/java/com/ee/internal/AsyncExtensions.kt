package com.ee.internal

import androidx.core.util.Consumer
import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.Dispatchers
import kotlin.coroutines.Continuation
import kotlin.coroutines.CoroutineContext

/// https://stackoverflow.com/questions/52869672/call-kotlin-suspend-function-in-java-class
@JvmOverloads
fun <T> getContinuation(onSuccess: Consumer<T>,
                        onFailure: Consumer<Throwable>,
                        dispatcher: CoroutineDispatcher = Dispatchers.Default): Continuation<T> {
    return object : Continuation<T> {
        override val context: CoroutineContext
            get() = dispatcher

        override fun resumeWith(result: Result<T>) {
            if (result.isSuccess) {
                onSuccess.accept(result.getOrNull())
            } else {
                onFailure.accept(result.exceptionOrNull())
            }
        }
    }
}