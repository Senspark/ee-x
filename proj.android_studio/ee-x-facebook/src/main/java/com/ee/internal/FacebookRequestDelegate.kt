package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.IMessageBridge
import com.ee.Thread
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.share.widget.GameRequestDialog
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable

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

    @InternalSerializationApi
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