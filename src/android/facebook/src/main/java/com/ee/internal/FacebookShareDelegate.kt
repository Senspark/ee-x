package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.IMessageBridge
import com.ee.Thread
import com.facebook.FacebookCallback
import com.facebook.FacebookException
import com.facebook.share.Sharer

/**
 * Created by eps on 3/20/18.
 */
internal class FacebookShareDelegate(
    private val _bridge: IMessageBridge,
    private val _tag: Int) : FacebookCallback<Sharer.Result> {
    companion object {
        private const val kPrefix = "FacebookShareDelegate"
    }

    private val kOnSuccess: String
        @AnyThread get() = "${kPrefix}_onSuccess_$_tag"

    private val kOnFailure: String
        @AnyThread get() = "${kPrefix}_onFailure_$_tag"

    private val kOnCancel: String
        @AnyThread get() = "${kPrefix}_onCancel_$_tag"

    override fun onSuccess(result: Sharer.Result) {
        Thread.checkMainThread()
        _bridge.callCpp(kOnSuccess, "")
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