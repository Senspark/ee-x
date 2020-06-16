package com.ee.facebook;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.Thread;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.login.LoginResult;

/**
 * Created by eps on 3/21/18.
 */

class FacebookLoginDelegate implements FacebookCallback<LoginResult> {
    private final IMessageBridge _bridge;
    private final int _tag;

    FacebookLoginDelegate(@NonNull IMessageBridge bridge, int tag) {
        _bridge = bridge;
        _tag = tag;
    }

    @NonNull
    private String k__onSuccess() {
        return "FacebookLoginDelegate_onSuccess_" + _tag;
    }

    @NonNull
    private String k__onFailure() {
        return "FacebookLoginDelegate_onFailure_" + _tag;
    }

    @NonNull
    private String k__onCancel() {
        return "FacebookLoginDelegate_onCancel_" + _tag;
    }

    @Override
    public void onSuccess(LoginResult result) {
        Thread.checkMainThread();
        _bridge.callCpp(k__onSuccess(), Facebook.convertAccessTokenToString(result.getAccessToken()));
    }

    @Override
    public void onError(FacebookException error) {
        Thread.checkMainThread();
        _bridge.callCpp(k__onFailure(), error.getLocalizedMessage());
    }

    @Override
    public void onCancel() {
        Thread.checkMainThread();
        _bridge.callCpp(k__onCancel());
    }
}
