package com.ee.twitter;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.support.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;

import java.io.File;
import java.util.Map;

import com.twitter.sdk.android.core.Callback;
import com.twitter.sdk.android.core.identity.TwitterAuthClient;
import com.twitter.sdk.android.tweetcomposer.ComposerActivity;
import com.twitter.sdk.android.core.TwitterConfig;
import com.twitter.sdk.android.core.TwitterAuthConfig;
import com.twitter.sdk.android.core.TwitterCore;
import com.twitter.sdk.android.core.TwitterSession;
import com.twitter.sdk.android.core.Result;
import com.twitter.sdk.android.core.TwitterException;


/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Twitter implements PluginProtocol {
    private static final String k__initialize      = "twitter_initialize";
    private static final String k__shareContent    = "twitter_shareContent";
    private static final String k__shareScreenShot = "twitter_shareScreenShot";

    private static final String k__onFailure = "twitter_onFailure";

    private static final String k__key    = "twitter_key";
    private static final String k__secret = "twitter_secret";
    private static final String k__text   = "twitter_text";
    private static final String k__image  = "twitter_image";

    private Activity          _activity;
    private IMessageBridge    _bridge;
    private TwitterAuthClient _authClient;

    public Twitter() {
        _activity = null;
        _bridge = MessageBridge.getInstance();
        _authClient = null;

        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Twitter";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
    }

    @Override
    public void onStart() {
        //do nothing
    }

    @Override
    public void onStop() {
        //do nothing
    }

    @Override
    public void onResume() {
        //do nothing
    }

    @Override
    public void onPause() {
        //do nothing
    }

    @Override
    public void onDestroy() {
        _activity = null;
        _authClient = null;
    }

    @Override
    public void destroy() {
        deregisterHandlers();
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String key = dict.get(k__key).toString();
                String secret = dict.get(k__secret).toString();
                initialize(key, secret);
                return "";
            }
        }, k__initialize);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String text = dict.get(k__text).toString();
                shareContent(text);
                return "";
            }
        }, k__shareContent);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                String text = dict.get(k__text).toString();
                String image = dict.get(k__image).toString();
                shareScreenShot(text, image);
                return "";
            }
        }, k__shareScreenShot);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.deregisterHandler(k__initialize);
        bridge.deregisterHandler(k__shareContent);
        bridge.deregisterHandler(k__shareScreenShot);
    }

    private TwitterAuthClient getTwitterAuthClient() {
        if (_authClient == null) {
            _authClient = new TwitterAuthClient();
        }
        return _authClient;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        if (requestCode == getTwitterAuthClient().getRequestCode()) {
            getTwitterAuthClient().onActivityResult(requestCode, responseCode, data);
            return true;
        }
        return false;
    }

    private void initialize(final String key, final String secret) {
        TwitterConfig config = new TwitterConfig.Builder(_activity).twitterAuthConfig(new TwitterAuthConfig(key, secret)).debug(true).build();
        com.twitter.sdk.android.core.Twitter.initialize(config);
    }

    private void shareContent(final String text) {
        final TwitterSession session = TwitterCore.getInstance().getSessionManager().getActiveSession();
        if (session == null) {
            getTwitterAuthClient().authorize(_activity, new Callback<TwitterSession>() {
                @Override
                public void success(Result<TwitterSession> twitterSessionResult) {
                    shareContentAfterCheckAuthen(text);
                }

                @Override
                public void failure(TwitterException e) {
                    _bridge.callCpp(k__onFailure);
                }
            });
        } else {
            shareContentAfterCheckAuthen(text);
        }
    }

    private void shareScreenShot(final String text, final String image) {
        final Uri imageUri = Uri.fromFile(new File(image));
        final TwitterSession session = TwitterCore.getInstance().getSessionManager().getActiveSession();
        if (session == null) {
            getTwitterAuthClient().authorize(_activity, new Callback<TwitterSession>() {
                @Override
                public void success(Result<TwitterSession> twitterSessionResult) {
                    shareScreenShotAfterCheckAuthen(text, imageUri);
                }

                @Override
                public void failure(TwitterException e) {
                    _bridge.callCpp(k__onFailure);
                }
            });
        } else {
            shareScreenShotAfterCheckAuthen(text, imageUri);
        }
    }

    private void shareScreenShotAfterCheckAuthen(final String text, final Uri imageUri) {
        final TwitterSession session = TwitterCore.getInstance().getSessionManager().getActiveSession();
        final Intent intent = new ComposerActivity.Builder(_activity).session(session).image(imageUri).text(text).createIntent();
        _activity.startActivity(intent);
    }

    private void shareContentAfterCheckAuthen(final String text) {
        final TwitterSession session = TwitterCore.getInstance().getSessionManager().getActiveSession();
        final Intent intent = new ComposerActivity.Builder(_activity).session(session).text(text).createIntent();
        _activity.startActivity(intent);
    }
}
