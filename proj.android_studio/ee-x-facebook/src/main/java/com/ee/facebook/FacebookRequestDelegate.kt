package com.ee.facebook

import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.internal.Thread
import com.ee.core.internal.serialize
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.share.widget.GameRequestDialog
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by eps on 3/21/18.
 */
internal class FacebookRequestDelegate(
    private val _bridge: IMessageBridge,
    private val _tag: Int) : FacebookCallback<GameRequestDialog.Result> {
    private val k__onSuccess: String
        @AnyThread get() = "FacebookRequestDelegate_onSuccess_$_tag"

    private val k__onFailure: String
        @AnyThread get() = "FacebookRequestDelegate_onFailure_$_tag"

    private val k__onCancel: String
        @AnyThread get() = "FacebookRequestDelegate_onCancel_$_tag"

    @ImplicitReflectionSerializer
    @UnstableDefault
    override fun onSuccess(result: GameRequestDialog.Result) {
        Thread.checkMainThread()
        @Serializable
        @Suppress("unused")
        class Response(
            val requestId: String,
            val requestRecipients: List<String>
        )

        val response = Response(result.requestId, result.requestRecipients)
        _bridge.callCpp(k__onSuccess, response.serialize())
    }

    override fun onError(error: FacebookException) {
        Thread.checkMainThread()
        _bridge.callCpp(k__onFailure, error.localizedMessage ?: "")
    }

    override fun onCancel() {
        Thread.checkMainThread()
        _bridge.callCpp(k__onCancel)
    }
}