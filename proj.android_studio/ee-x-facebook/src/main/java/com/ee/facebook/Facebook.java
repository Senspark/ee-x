package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.widget.ShareDialog;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Facebook implements PluginProtocol {

    private static final String k__facebook_share_link_content    = "__facebook_share_link_content";
    private static final String k__facebook_share_photo_content    = "__facebook_share_photo_content";
    private static final String k__onResult    = "__onResult";

    private static final Logger _logger = new Logger(Facebook.class.getName());

    private Context                             _context;
    private Activity                            _activity;

    private static CallbackManager callbackManager;

    public Facebook(Context context) {
        Utils.checkMainThread();
        _context = context;
        _activity = null;

        callbackManager = CallbackManager.Factory.create();
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
        _context = null;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        callbackManager.onActivityResult(requestCode, responseCode, data);
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

         bridge.registerHandler(new MessageHandler() {
             @NonNull
             @Override
             public String handle(@NonNull String message) {
                 shareLinkContent(message);
                 return "";
             }
         }, k__facebook_share_link_content);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                sharePhotoContent(message);
                return "";
            }
        }, k__facebook_share_photo_content);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

         bridge.deregisterHandler(k__facebook_share_link_content);
        bridge.deregisterHandler(k__facebook_share_photo_content);
    }

    @SuppressWarnings("WeakerAccess")
    public void shareLinkContent(@NonNull String url) {

        ShareLinkContent shareContent = new ShareLinkContent.Builder()
                .setContentUrl(Uri.parse(url))
                .build();

        ShareDialog shareDialog = new ShareDialog(_activity);
        shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);

        shareDialog.registerCallback(callbackManager, new FacebookCallback<Sharer.Result>(){
            @Override
            public void onSuccess(Sharer.Result result) {
                if(result.getPostId() != null) {

                    Utils.checkMainThread();

                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onResult, "true");
                }
            }

            @Override
            public void onCancel() {
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onResult, "false");
            }

            @Override
            public void onError(FacebookException error) {
                MessageBridge bridge = MessageBridge.getInstance();
                bridge.callCpp(k__onResult, "false");
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public void sharePhotoContent(@NonNull String photo) {


    }
}
