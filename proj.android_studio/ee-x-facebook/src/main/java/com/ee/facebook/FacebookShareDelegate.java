package com.ee.facebook;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.Utils;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.share.Sharer;

/**
 * Created by eps on 3/20/18.
 */

public class FacebookShareDelegate implements FacebookCallback<Sharer.Result> {
    private final IMessageBridge _bridge;
    private final int _tag;

    FacebookShareDelegate(@NonNull IMessageBridge bridge, int tag) {
        _bridge = bridge;
        _tag = tag;
    }

    @NonNull
    private String k__onSuccess() {
        return "FacebookShareDelegate_onSuccess_" + _tag;
    }

    @NonNull
    private String k__onFailure() {
        return "FacebookShareDelegate_onFailure_" + _tag;
    }

    @NonNull
    private String k__onCancel() {
        return "FacebookShareDelegate_onCancel_" + _tag;
    }

    @Override
    public void onSuccess(Sharer.Result result) {
        Utils.checkMainThread();
        _bridge.callCpp(k__onSuccess(), "");
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
