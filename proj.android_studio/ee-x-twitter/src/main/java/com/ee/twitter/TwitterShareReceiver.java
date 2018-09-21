package com.ee.twitter;

import com.ee.core.IMessageBridge;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import com.ee.core.MessageBridge;
import com.twitter.sdk.android.tweetcomposer.TweetUploadService;

public class TwitterShareReceiver extends BroadcastReceiver {

    private static final String k__onSuccess        = "Twitter_onSuccess";
    private static final String k__onFailure        = "Twitter_onFailure";
    private static final String k__onCancel         = "Twitter_onCancel";

    @Override
    public void onReceive(Context context, Intent intent) {
        if (TweetUploadService.UPLOAD_SUCCESS.equals(intent.getAction())) {
            // success
            //final Long tweetId = intent.getExtras().getLong(TweetUploadService.EXTRA_TWEET_ID);

            IMessageBridge brige = MessageBridge.getInstance();
            MessageBridge.getInstance().callCpp(k__onSuccess);


        } else if (TweetUploadService.UPLOAD_FAILURE.equals(intent.getAction())) {
            // failure
            //final Intent retryIntent = intent.getExtras().getParcelable(TweetUploadService.EXTRA_RETRY_INTENT);

            IMessageBridge brige = MessageBridge.getInstance();
            MessageBridge.getInstance().callCpp(k__onFailure);


        } else if (TweetUploadService.TWEET_COMPOSE_CANCEL.equals(intent.getAction())) {
            // cancel

            IMessageBridge brige = MessageBridge.getInstance();
            MessageBridge.getInstance().callCpp(k__onCancel);

        }
    }

}
