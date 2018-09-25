package com.ee.twitter;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.support.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;

import java.io.File;
import java.util.List;
import java.util.Map;

import com.twitter.sdk.android.core.Callback;
import com.twitter.sdk.android.core.TwitterApiException;
import com.twitter.sdk.android.core.identity.TwitterLoginButton;
import com.twitter.sdk.android.tweetcomposer.TweetComposer;
import com.twitter.sdk.android.tweetcomposer.ComposerActivity;
import com.twitter.sdk.android.core.TwitterConfig;
import com.twitter.sdk.android.core.TwitterAuthConfig;
import com.twitter.sdk.android.core.TwitterCore;
import com.twitter.sdk.android.core.TwitterSession;
import com.twitter.sdk.android.core.Callback;
import com.twitter.sdk.android.core.Result;
import com.twitter.sdk.android.core.TwitterException;

import android.support.v4.content.FileProvider;

import retrofit2.Call;
import retrofit2.Response;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Twitter implements PluginProtocol {
    private static final String k__initialize       = "Twitter_initialize";
    private static final String k__shareContent     = "Twitter_shareContent";
    private static final String k__shareScreenShot  = "Twitter_shareScreenShot";

    private static final String k__onSuccess        = "Twitter_onSuccess";
    private static final String k__onFailure        = "Twitter_onFailure";
    private static final String k__onCancel         = "Twitter_onCancel";

    private static final String k__text             = "twitter_text";
    private static final String k__image            = "twitter_image";

    private Activity _activity;
    private IMessageBridge _bridge;
    TwitterLoginButton _twiterLoginButton;

    public Twitter() {
        _activity = null;
        _bridge = MessageBridge.getInstance();
        _twiterLoginButton = null;

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
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        _activity = null;
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
                initialize();
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
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {

        if (requestCode == TwitterAuthConfig.DEFAULT_AUTH_REQUEST_CODE) {
            if (_twiterLoginButton != null) {
                _twiterLoginButton.onActivityResult(requestCode, responseCode, data);
                _twiterLoginButton = null;
            }
            return true;
        }
        else {
            return false;
        }
    }


    private void initialize() {

        TwitterConfig config = new TwitterConfig.Builder(_activity)
                .twitterAuthConfig(new TwitterAuthConfig("WgRdd92MiYn1L4JiskaGfl8wK", "FYlWKjtQXDedTRaiuyWF9nl6wIWu243QaI0BMIitrW2GMna9Aq"))
                .debug(true)
                .build();
        com.twitter.sdk.android.core.Twitter.initialize(config);

    }

    private void shareContent(String text) {

        final TwitterSession session = TwitterCore.getInstance().getSessionManager()
                .getActiveSession();

        if (session == null) {

            _twiterLoginButton = new TwitterLoginButton(_activity);
            final String ftext = text;


            _twiterLoginButton.setCallback(new Callback<TwitterSession>() {

                @Override
                public void success(Result<TwitterSession> result) {
                    shareContentAfterCheckAuthen (ftext);

                }

                @Override
                public void failure(TwitterException exception) {
                    _bridge.callCpp(k__onFailure);

                }
            });

            _twiterLoginButton.performClick();

        }
        else {
            shareContentAfterCheckAuthen (text);
        }


//        TweetComposer.Builder builder = new TweetComposer.Builder(_activity)
//                .text(text);
//        builder.show();

    }

    private void shareScreenShot(String text, final String image) {

        final Uri imageUri;
        //if (Build.VERSION.SDK_INT >= 24) {
        //    imageUri = FileProvider.getUriForFile(_activity, "com.ee.twitter.provider", new File(image));
        //}
        //else {
            imageUri = Uri.fromFile(new File(image));
        //}

        final TwitterSession session = TwitterCore.getInstance().getSessionManager()
                .getActiveSession();

        if (session == null) {

            _twiterLoginButton = new TwitterLoginButton(_activity);
            final String ftext = text;


            _twiterLoginButton.setCallback(new Callback<TwitterSession>() {

                @Override
                public void success(Result<TwitterSession> result) {
                    shareScreenShotAfterCheckAuthen(ftext, imageUri);

                }

                @Override
                public void failure(TwitterException exception) {
                    _bridge.callCpp(k__onFailure);

                }
            });

            _twiterLoginButton.performClick();

        }
        else {
            shareScreenShotAfterCheckAuthen(text, imageUri);
        }


    }

    private void shareScreenShotAfterCheckAuthen(String text, Uri imageUri) {

        final TwitterSession session = TwitterCore.getInstance().getSessionManager()
                .getActiveSession();

        final Intent intent = new ComposerActivity.Builder(_activity)
                .session(session)
                .image(imageUri)
                .text(text)
                .createIntent();
        _activity.startActivity(intent);

//        TweetComposer.Builder builder = new TweetComposer.Builder(_activity)
//                .text(text)
//                .image(imageUri);
//        builder.show();


    }

    private void shareContentAfterCheckAuthen(String text) {

        final TwitterSession session = TwitterCore.getInstance().getSessionManager()
                .getActiveSession();

        final Intent intent = new ComposerActivity.Builder(_activity)
                .session(session)
                .text(text)
                .createIntent();
        _activity.startActivity(intent);

    }

}
