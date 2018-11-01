package com.ee.twitter;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.ee.core.MessageBridge;
import com.twitter.sdk.android.tweetcomposer.TweetUploadService;

public class TwitterShareReceiver extends BroadcastReceiver {

    private static final String k__onSuccess = "twitter_onSuccess";
    private static final String k__onFailure = "twitter_onFailure";
    private static final String k__onCancel  = "twitter_onCancel";

    @Override
    public void onReceive(Context context, Intent intent) {
        if (TweetUploadService.UPLOAD_SUCCESS.equals(intent.getAction())) {
            MessageBridge.getInstance().callCpp(k__onSuccess);
        } else if (TweetUploadService.UPLOAD_FAILURE.equals(intent.getAction())) {
            MessageBridge.getInstance().callCpp(k__onFailure);
        } else if (TweetUploadService.TWEET_COMPOSE_CANCEL.equals(intent.getAction())) {
            MessageBridge.getInstance().callCpp(k__onCancel);
        }
    }
}
