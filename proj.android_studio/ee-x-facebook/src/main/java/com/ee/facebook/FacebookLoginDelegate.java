package com.ee.facebook;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.Utils;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.login.LoginResult;

/**
 * Created by eps on 3/21/18.
 */

public class FacebookLoginDelegate implements FacebookCallback<LoginResult> {
    private int            _tag;
    private IMessageBridge _bridge;

    FacebookLoginDelegate(IMessageBridge bridge, int tag) {
        _bridge = bridge;
        _tag = tag;
    }

    @NonNull
    private String k__onSuccess() {
        return "FacebookLoginDelegate_onSuccess_" + String.valueOf(_tag);
    }

    @NonNull
    private String k__onFailure() {
        return "FacebookLoginDelegate_onFailure_" + String.valueOf(_tag);
    }

    @NonNull
    private String k__onCancel() {
        return "FacebookLoginDelegate_onCancel_" + String.valueOf(_tag);
    }

    @Override
    public void onSuccess(LoginResult result) {
        Utils.checkMainThread();
        _bridge.callCpp(k__onSuccess(), Facebook.convertAccessTokenToString(result.getAccessToken()));
    }

    @Override
    public void onError(FacebookException error) {
        Utils.checkMainThread();
        _bridge.callCpp(k__onFailure(), error.getLocalizedMessage());
    }

    @Override
    public void onCancel() {
        Utils.checkMainThread();
        _bridge.callCpp(k__onCancel());
    }
}
