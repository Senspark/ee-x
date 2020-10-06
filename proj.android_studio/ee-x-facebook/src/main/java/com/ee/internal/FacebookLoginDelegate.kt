package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.FacebookBridge
import com.ee.IMessageBridge
import com.ee.Thread
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.login.LoginResult
import kotlinx.serialization.InternalSerializationApi

/**
 * Created by eps on 3/21/18.
 */
@InternalSerializationApi
internal class FacebookLoginDelegate(
    private val _bridge: IMessageBridge,
    private val _tag: Int) : FacebookCallback<LoginResult> {
    companion object {
        private const val kPrefix = "FacebookLoginDelegate"
    }

    private val kOnSuccess: String
        @AnyThread get() = "${kPrefix}_onSuccess_$_tag"

    private val kOnFailure: String
        @AnyThread get() = "${kPrefix}_onFailure_$_tag"

    private val kOnCancel: String
        @AnyThread get() = "${kPrefix}_onCancel_$_tag"

    override fun onSuccess(result: LoginResult) {
        Thread.checkMainThread()
        _bridge.callCpp(kOnSuccess, FacebookBridge.convertAccessTokenToString(result.accessToken))
    }

    override fun onError(error: FacebookException) {
        Thread.checkMainThread()
        _bridge.callCpp(kOnFailure, error.localizedMessage ?: "")
    }

    override fun onCancel() {
        Thread.checkMainThread()
        _bridge.callCpp(kOnCancel)
    }
}