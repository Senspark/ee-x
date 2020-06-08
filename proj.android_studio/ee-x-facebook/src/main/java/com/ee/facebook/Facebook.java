package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.util.SparseArray;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.IPlugin;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.facebook.AccessToken;
import com.facebook.AccessTokenTracker;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.HttpMethod;
import com.facebook.Profile;
import com.facebook.ProfileTracker;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.Sharer;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.model.ShareContent;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.model.ShareVideo;
import com.facebook.share.model.ShareVideoContent;
import com.facebook.share.widget.GameRequestDialog;
import com.facebook.share.widget.ShareDialog;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Facebook implements IPlugin {
    private static final String k__registerNotifications = "Facebook_registerNotifications";
    private static final String k__isLoggedIn = "Facebook_isLoggedIn";
    private static final String k__logIn = "Facebook_logIn";
    private static final String k__logOut = "Facebook_logOut";
    private static final String k__getAccessToken = "Facebook_getAccessToken";
    private static final String k__onProfileChanged = "Facebook_onProfileChanged";
    private static final String k__graphRequest = "Facebook_graphRequest";
    private static final String k__sendRequest = "Facebook_sendRequest";
    private static final String k__shareLinkContent = "Facebook_shareLinkContent";
    private static final String k__sharePhotoContent = "Facebook_sharePhotoContent";
    private static final String k__shareVideoContent = "Facebook_shareVideoContent";

    private static final Logger _logger = new Logger(Facebook.class.getName());

    private IMessageBridge _bridge;
    private Activity _activity;
    private CallbackManager _callbackManager;
    private AccessTokenTracker _accessTokenTracker;
    private ProfileTracker _profileTracker;

    public Facebook(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Utils.checkMainThread();
        _bridge = bridge;
        _callbackManager = CallbackManager.Factory.create();
        _accessTokenTracker = new AccessTokenTracker() {
            @Override
            protected void onCurrentAccessTokenChanged(AccessToken oldAccessToken, AccessToken currentAccessToken) {
                _logger.debug("onCurrentAccessTokenChanged");
            }
        };
        _profileTracker = new ProfileTracker() {
            @Override
            protected void onCurrentProfileChanged(Profile oldProfile, Profile currentProfile) {
                _logger.debug("onCurrentProfileChanged");
                Map<String, Object> dict = new HashMap<>();
                if (currentProfile != null) {
                    dict.put("userId", currentProfile.getId());
                    dict.put("firstName", currentProfile.getFirstName());
                    dict.put("middleName",
                        currentProfile.getMiddleName() == null ? "" : currentProfile.getMiddleName());
                    dict.put("lastName", currentProfile.getLastName());
                    dict.put("name", currentProfile.getName());
                    dict.put("picture", currentProfile.getProfilePictureUri(128, 128).toString());
                }
                // FIXME: may cause java.lang.UnsatisfiedLinkError if C++ library has not loaded yet.
                // _bridge.callCpp(k__onProfileChanged, JsonUtils.convertDictionaryToString(dict));
            }
        };

        // com.facebook.UserSettingsManager: Please set a value for AutoLogAppEventsEnabled.
        // Set the flag to TRUE if you want to collect app install, app launch and in-app purchase events automatically.
        // To request user consent before collecting data, set the flag value to FALSE, then change to TRUE once user consent is received.
        // Learn more: https://developers.facebook.com/docs/app-events/getting-started-app-events-android#disable-auto-events.
        FacebookSdk.setAutoLogAppEventsEnabled(true);

        // com.facebook.UserSettingsManager: You haven't set a value for AdvertiserIDCollectionEnabled.
        // Set the flag to TRUE if you want to collect Advertiser ID for better advertising and analytics results.
        // To request user consent before collecting data, set the flag value to FALSE, then change to TRUE once user consent is received.
        // Learn more: https://developers.facebook.com/docs/app-events/getting-started-app-events-android#disable-auto-events.
        FacebookSdk.setAdvertiserIDCollectionEnabled(true);

        registerHandlers();
    }

    @NonNull
    static String convertAccessTokenToString(@Nullable AccessToken token) {
        if (token == null) {
            return "";
        }
        Map<String, Object> dict = new HashMap<>();
        dict.put("token", token.getToken());
        dict.put("applicationId", token.getApplicationId());
        dict.put("userId", token.getUserId());
        return Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict));
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Facebook";
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
        Utils.checkMainThread();
        deregisterHandlers();
        _bridge = null;
        _callbackManager = null;
        _accessTokenTracker = null;
        _profileTracker = null;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return _callbackManager.onActivityResult(requestCode, responseCode, data);
    }

    @SuppressWarnings("WeakerAccess")
    public void registerNotifications() {
        _profileTracker.startTracking();
        _accessTokenTracker.startTracking();
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__registerNotifications, message -> {
            registerNotifications();
            return "";
        });

        _bridge.registerHandler(k__isLoggedIn, message ->
            Utils.toString(isLoggedIn()));

        _bridge.registerHandler(k__logIn, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            List<String> permissions = (List<String>) dict.get("permissions");
            Integer tag = (Integer) dict.get("tag");
            assertThat(permissions).isNotNull();
            assertThat(tag).isNotNull();

            logIn(permissions, new FacebookLoginDelegate(_bridge, tag));
            return "";
        });

        _bridge.registerHandler(k__logOut, message -> {
            logOut();
            return "";
        });

        _bridge.registerHandler(k__getAccessToken, message -> {
            AccessToken token = getAccessToken();
            return convertAccessTokenToString(token);
        });

        _bridge.registerHandler(k__graphRequest, this::graphRequest);

        _bridge.registerHandler(k__sendRequest, this::sendRequest);

        _bridge.registerHandler(k__shareLinkContent, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            Integer tag = (Integer) dict.get("tag");
            String url = (String) dict.get("url");
            assertThat(tag).isNotNull();
            assertThat(url).isNotNull();

            shareLinkContent(url, new FacebookShareDelegate(_bridge, tag));
            return "";
        });

        _bridge.registerHandler(k__sharePhotoContent, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            Integer tag = (Integer) dict.get("tag");
            String url = (String) dict.get("url");
            assertThat(tag).isNotNull();
            assertThat(url).isNotNull();

            sharePhotoContent(url, new FacebookShareDelegate(_bridge, tag));
            return "";
        });

        _bridge.registerHandler(k__shareVideoContent, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            Integer tag = (Integer) dict.get("tag");
            String url = (String) dict.get("url");
            assertThat(tag).isNotNull();
            assertThat(url).isNotNull();

            shareVideoContent(url, new FacebookShareDelegate(_bridge, tag));
            return "";
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__registerNotifications);
        _bridge.deregisterHandler(k__isLoggedIn);
        _bridge.deregisterHandler(k__logIn);
        _bridge.deregisterHandler(k__logOut);
        _bridge.deregisterHandler(k__getAccessToken);
        _bridge.deregisterHandler(k__graphRequest);
        _bridge.deregisterHandler(k__sendRequest);
        _bridge.deregisterHandler(k__shareLinkContent);
        _bridge.deregisterHandler(k__sharePhotoContent);
        _bridge.deregisterHandler(k__shareVideoContent);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isLoggedIn() {
        return AccessToken.getCurrentAccessToken() != null;
    }

    @SuppressWarnings("WeakerAccess")
    public void logIn(@NonNull List<String> permissions, @NonNull FacebookCallback<LoginResult> callback) {
        LoginManager.getInstance().registerCallback(_callbackManager, callback);
        LoginManager.getInstance().logInWithReadPermissions(_activity, permissions);
    }

    @SuppressWarnings("WeakerAccess")
    public void logOut() {
        LoginManager.getInstance().logOut();
    }

    @Nullable
    public AccessToken getAccessToken() {
        return AccessToken.getCurrentAccessToken();
    }

    @NonNull
    private String k__onGraphSuccess(int tag) {
        return "FacebookGraphDelegate_onSuccess_" + tag;
    }

    @NonNull
    private String k__onGraphFailure(int tag) {
        return "FacebookGraphDelegate_onFailure_" + tag;
    }

    @NonNull
    private String graphRequest(@NonNull String message) {
        Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
        assertThat(dict).isNotNull();

        Integer tag = (Integer) dict.get("tag");
        String path = (String) dict.get("path");
        @SuppressWarnings("unchecked")
        Map<String, Object> parameters_ = (Map<String, Object>) dict.get("parameters");
        assertThat(tag).isNotNull();
        assertThat(path).isNotNull();
        assertThat(parameters_).isNotNull();

        Bundle parameters = new Bundle();
        for (Map.Entry<String, Object> entry : parameters_.entrySet()) {
            parameters.putString(entry.getKey(), (String) entry.getValue());
        }
        GraphRequest.Callback callback = response -> {
            if (response.getError() != null) {
                _bridge.callCpp(k__onGraphFailure(tag), response.getError().getErrorMessage());
            } else {
                _bridge.callCpp(k__onGraphSuccess(tag), response.getRawResponse());
            }
        };
        graphRequest(path, parameters, callback);
        return "";
    }

    @SuppressWarnings("WeakerAccess")
    public void graphRequest(@NonNull String path, @Nullable Bundle parameters, GraphRequest.Callback callback) {
        GraphRequest request = new GraphRequest(getAccessToken(), path, parameters, HttpMethod.GET, callback);
        request.executeAsync();
    }

    @NonNull
    private String sendRequest(@NonNull String message_) {
        Map<String, Object> dict = JsonUtils.convertStringToDictionary(message_);
        assertThat(dict).isNotNull();

        Integer tag = (Integer) dict.get("tag");
        Integer actionType = (Integer) dict.get("actionType");
        Integer filter = (Integer) dict.get("filter");
        assertThat(tag).isNotNull();
        assertThat(actionType).isNotNull();
        assertThat(filter).isNotNull();

        SparseArray<GameRequestContent.ActionType> actionTypes = new SparseArray<>();
        actionTypes.put(0, null);
        actionTypes.put(1, GameRequestContent.ActionType.SEND);
        actionTypes.put(2, GameRequestContent.ActionType.ASKFOR);
        actionTypes.put(3, GameRequestContent.ActionType.TURN);

        SparseArray<GameRequestContent.Filters> filters = new SparseArray<>();
        filters.put(0, null);
        filters.put(1, GameRequestContent.Filters.APP_USERS);
        filters.put(2, GameRequestContent.Filters.APP_NON_USERS);

        String title = (String) dict.get("title");
        @SuppressWarnings("unchecked")
        List<String> recipients = (List<String>) dict.get("recipients");
        String objectId = (String) dict.get("objectId");
        String data = (String) dict.get("data");
        String message = (String) dict.get("message");
        assertThat(message).isNotNull();

        sendRequest(
            actionTypes.get(actionType),
            filters.get(filter),
            title, recipients, objectId, data, message,
            new FacebookRequestDelegate(_bridge, tag));
        return "";
    }

    public void sendRequest(@Nullable GameRequestContent.ActionType actionType,
                            @Nullable GameRequestContent.Filters filter,
                            @Nullable String title,
                            @Nullable List<String> recipients,
                            @Nullable String objectId,
                            @Nullable String data,
                            @NonNull String message,
                            @NonNull FacebookCallback<GameRequestDialog.Result> delegate) {
        GameRequestContent content = new GameRequestContent.Builder()
            .setActionType(actionType)
            .setFilters(filter)
            .setTitle(title)
            .setObjectId(objectId)
            .setRecipients(recipients)
            .setData(data)
            .setMessage(message)
            .build();

        GameRequestDialog dialog = new GameRequestDialog(_activity);
        dialog.registerCallback(_callbackManager, delegate);
        dialog.show(content);
    }

    @SuppressWarnings("WeakerAccess")
    public void shareLinkContent(@NonNull String url, FacebookCallback<Sharer.Result> delegate) {
//        String encodedUrl;
//        try {
//            encodedUrl = URLEncoder.encode(url, "utf-8");
//        } catch (UnsupportedEncodingException e) {
//            encodedUrl = url;
//        }

        ShareLinkContent content = new ShareLinkContent.Builder()
            .setContentUrl(Uri.parse(url))
            .build();
        shareContent(content, delegate);
    }

    @SuppressWarnings("WeakerAccess")
    public void sharePhotoContent(@NonNull String url, FacebookCallback<Sharer.Result> delegate) {
        Bitmap image = BitmapFactory.decodeFile(url);
        SharePhoto photo = new SharePhoto.Builder().setBitmap(image).build();
        SharePhotoContent content = new SharePhotoContent.Builder().addPhoto(photo).build();
        shareContent(content, delegate);
    }

    @SuppressWarnings("WeakerAccess")
    public void shareVideoContent(@NonNull String url, FacebookCallback<Sharer.Result> delegate) {
        File video = new File(url);
        Uri videoFileUri = Uri.fromFile(video);
        ShareVideo videoContent = new ShareVideo.Builder().setLocalUrl(videoFileUri).build();
        ShareVideoContent content = new ShareVideoContent.Builder().setVideo(videoContent).build();
        shareContent(content, delegate);

    }

    private void shareContent(ShareContent content, FacebookCallback<Sharer.Result> delegate) {
        if (_activity == null) {
            return;
        }

        ShareDialog dialog = new ShareDialog(_activity);
        dialog.registerCallback(_callbackManager, delegate);
        dialog.show(content, ShareDialog.Mode.AUTOMATIC);
    }
}
