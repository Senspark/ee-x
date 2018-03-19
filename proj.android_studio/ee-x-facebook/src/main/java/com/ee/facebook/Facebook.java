package com.ee.facebook;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.facebook.AccessToken;
import com.facebook.AccessTokenTracker;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.Profile;
import com.facebook.ProfileTracker;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.model.ShareVideo;
import com.facebook.share.model.ShareVideoContent;
import com.facebook.share.widget.ShareDialog;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Facebook implements PluginProtocol {
    private static final String k__registerNotifications = "Facebook_registerNotifications";
    private static final String k__isLoggedIn            = "Facebook_isLoggedIn";
    private static final String k__logIn                 = "Facebook_logIn";
    private static final String k__logOut                = "Facebook_logOut";
    private static final String k__getAccessToken        = "Facebook_getAccessToken";
    private static final String k__getUserId             = "Facebook_getUserId";
    private static final String k__onLoginResult         = "Facebook_onLoginResult";
    private static final String k__onProfileChanged      = "Facebook_onProfileChanged";
    private static final String k__shareLinkContent      = "Facebook_shareLinkContent";
    private static final String k__sharePhotoContent     = "Facebook_sharePhotoContent";
    private static final String k__shareVideoContent     = "Facebook_shareVideoContent";
    private static final String k__onShareResult         = "Facebook_onShareResult";

    private static final Logger _logger = new Logger(Facebook.class.getName());

    private Activity                        _activity;
    private ShareDialog                     _shareDialog;
    private CallbackManager                 _callbackManager;
    private FacebookCallback<Sharer.Result> _shareCallback;
    private FacebookCallback<LoginResult>   _loginCallback;
    private AccessTokenTracker              _accessTokenTracker;
    private ProfileTracker                  _profileTracker;

    public Facebook() {
        Utils.checkMainThread();
        _activity = null;
        _shareDialog = null;
        _callbackManager = CallbackManager.Factory.create();
        _shareCallback = new FacebookCallback<Sharer.Result>() {
            @Override
            public void onSuccess(Sharer.Result result) {
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onShareResult, Utils.toString(true));
            }

            @Override
            public void onCancel() {
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onShareResult, Utils.toString(false));
            }

            @Override
            public void onError(FacebookException error) {
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onShareResult, Utils.toString(false));
            }
        };
        _loginCallback = new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(LoginResult loginResult) {
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onLoginResult, Utils.toString(true));
            }

            @Override
            public void onCancel() {
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onLoginResult, Utils.toString(false));
            }

            @Override
            public void onError(FacebookException error) {
                Utils.checkMainThread();
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onLoginResult, Utils.toString(false));
            }
        };
        _accessTokenTracker = new AccessTokenTracker() {
            @Override
            protected void onCurrentAccessTokenChanged(AccessToken oldAccessToken, AccessToken
                    currentAccessToken) {
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
                    dict.put("middleName", currentProfile.getMiddleName() == null ? "" :
                            currentProfile.getMiddleName());
                    dict.put("lastName", currentProfile.getLastName());
                    dict.put("name", currentProfile.getName());
                    dict.put("picture", currentProfile.getProfilePictureUri(128, 128).toString());
                }
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onProfileChanged, JsonUtils.convertDictionaryToString(dict));
            }
        };
        LoginManager.getInstance().registerCallback(_callbackManager, _loginCallback);
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Facebook";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
        _shareDialog = new ShareDialog(activity);
        _shareDialog.registerCallback(_callbackManager, _shareCallback);
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
        _shareDialog = null;
        _activity = null;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
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
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                registerNotifications();
                return "";
            }
        }, k__registerNotifications);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isLoggedIn());
            }
        }, k__isLoggedIn);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                List<Object> permissions_ = JsonUtils.convertStringToArray(message);
                List<String> permissions = new LinkedList<>();
                for (Object object : permissions_) {
                    permissions.add((String) object);
                }
                logIn(permissions);
                return "";
            }
        }, k__logIn);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                logOut();
                return "";
            }
        }, k__logOut);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getAccessToken();
            }
        }, k__getAccessToken);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getUserId();
            }
        }, k__getUserId);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                shareLinkContent(message);
                return "";
            }
        }, k__shareLinkContent);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                sharePhotoContent(message);
                return "";
            }
        }, k__sharePhotoContent);


        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                shareVideoContent(message);
                return "";
            }
        }, k__shareVideoContent);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__shareLinkContent);
        bridge.deregisterHandler(k__sharePhotoContent);
        bridge.deregisterHandler(k__shareVideoContent);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isLoggedIn() {
        return AccessToken.getCurrentAccessToken() != null;
    }

    @SuppressWarnings("WeakerAccess")
    public void logIn(@NonNull List<String> permissions) {
        LoginManager.getInstance().logInWithReadPermissions(_activity, permissions);
    }

    @SuppressWarnings("WeakerAccess")
    public void logOut() {
        LoginManager.getInstance().logOut();
    }

    @NonNull
    public String getAccessToken() {
        if (!isLoggedIn()) {
            return "";
        }
        return AccessToken.getCurrentAccessToken().getToken();
    }

    @NonNull
    public String getUserId() {
        if (!isLoggedIn()) {
            return "";
        }
        return AccessToken.getCurrentAccessToken().getUserId();
    }

    @SuppressWarnings("WeakerAccess")
    public void shareLinkContent(@NonNull String url) {
        ShareLinkContent shareContent = new ShareLinkContent.Builder().setContentUrl(Uri.parse
                (url)).build();

        _shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);
    }

    @SuppressWarnings("WeakerAccess")
    public void sharePhotoContent(@NonNull String url) {
        Bitmap image = BitmapFactory.decodeFile(url);

        SharePhoto photo = new SharePhoto.Builder().setBitmap(image).build();

        SharePhotoContent shareContent = new SharePhotoContent.Builder().addPhoto(photo).build();

        _shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);
    }

    @SuppressWarnings("WeakerAccess")
    public void shareVideoContent(@NonNull String url) {
        _logger.debug("shareVideoContent: url = " + url);

        File video = new File(url);
        Uri videoFileUri = Uri.fromFile(video);

        ShareVideo videoContent = new ShareVideo.Builder().setLocalUrl(videoFileUri).build();

        ShareVideoContent shareContent = new ShareVideoContent.Builder().setVideo(videoContent)
                .build();

        _shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);
    }
}
