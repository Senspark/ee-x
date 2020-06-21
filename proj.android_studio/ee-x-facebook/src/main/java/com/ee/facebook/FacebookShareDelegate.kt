package com.ee.facebook

import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.internal.Thread
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.share.Sharer

/**
 * Created by eps on 3/20/18.
 */
internal class FacebookShareDelegate(
    private val _bridge: IMessageBridge,
    private val _tag: Int) : FacebookCallback<Sharer.Result> {
    private val k__onSuccess: String
        @AnyThread get() = "FacebookShareDelegate_onSuccess_$_tag"

    private val k__onFailure: String
        @AnyThread get() = "FacebookShareDelegate_onFailure_$_tag"

    private val k__onCancel: String
        @AnyThread get() = "FacebookShareDelegate_onCancel_$_tag"

    override fun onSuccess(result: Sharer.Result) {
        Thread.checkMainThread()
        _bridge.callCpp(k__onSuccess, "")
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