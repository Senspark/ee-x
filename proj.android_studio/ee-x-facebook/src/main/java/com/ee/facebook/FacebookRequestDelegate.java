package com.ee.facebook;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.share.widget.GameRequestDialog;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

/**
 * Created by eps on 3/21/18.
 */

class FacebookRequestDelegate implements FacebookCallback<GameRequestDialog.Result> {
    private IMessageBridge _bridge;
    private int _tag;

    FacebookRequestDelegate(IMessageBridge bridge, int tag) {
        _bridge = bridge;
        _tag = tag;
    }

    @NonNull
    private String k__onSuccess() {
        return "FacebookRequestDelegate_onSuccess_" + _tag;
    }

    @NonNull
    private String k__onFailure() {
        return "FacebookRequestDelegate_onFailure_" + _tag;
    }

    @NonNull
    private String k__onCancel() {
        return "FacebookRequestDelegate_onCancel_" + _tag;
    }

    @Override
    public void onSuccess(GameRequestDialog.Result result) {
        Utils.checkMainThread();
        Map<String, Object> dict = new HashMap<>();
        dict.put("requestId", result.getRequestId());
        dict.put("requestRecipients", result.getRequestRecipients());
        _bridge.callCpp(k__onSuccess(), Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict)));
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
