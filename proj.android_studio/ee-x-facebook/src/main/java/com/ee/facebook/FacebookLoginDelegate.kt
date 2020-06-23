package com.ee.facebook

import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.internal.Thread
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.login.LoginResult
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault

/**
 * Created by eps on 3/21/18.
 */
@ImplicitReflectionSerializer
@UnstableDefault
internal class FacebookLoginDelegate(
    private val _bridge: IMessageBridge,
    private val _tag: Int) : FacebookCallback<LoginResult> {
    private val k__onSuccess: String
        @AnyThread get() = "FacebookLoginDelegate_onSuccess_$_tag"

    private val k__onFailure: String
        @AnyThread get() = "FacebookLoginDelegate_onFailure_$_tag"

    private val k__onCancel: String
        @AnyThread get() = "FacebookLoginDelegate_onCancel_$_tag"

    override fun onSuccess(result: LoginResult) {
        Thread.checkMainThread()
        _bridge.callCpp(k__onSuccess, Facebook.convertAccessTokenToString(result.accessToken))
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