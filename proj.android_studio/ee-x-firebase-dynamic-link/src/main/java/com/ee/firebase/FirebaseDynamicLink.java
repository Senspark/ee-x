package com.ee.firebase;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.dynamiclinks.FirebaseDynamicLinks;
import com.google.firebase.dynamiclinks.PendingDynamicLinkData;

import java.util.concurrent.Executor;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class FirebaseDynamicLink implements PluginProtocol {
    private static final String k__setupReceiver = "Firebase_DynamicLink_setupReceiver";
    private static final String k__onReceiveDynamicLink          = "Firebase_DynamicLink_OnReceiveDynamicLink";

    private static final Logger _logger = new Logger(FirebaseDynamicLink.class.getName());

    private Activity _activity;

    public FirebaseDynamicLink()
    {
        Utils.checkMainThread();
        _activity = null;
        registerHandlers();
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                assert _activity != null;
                setupReceiver();
                return "";
            }
        }, k__setupReceiver);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__setupReceiver);
    }

    void setupReceiver()
    {
        _logger.debug("Deep link is setupReceiver: ");
        FirebaseDynamicLinks.getInstance()
                .getDynamicLink(_activity.getIntent())
                .addOnSuccessListener(_activity, new OnSuccessListener<PendingDynamicLinkData>() {
                    @Override
                    public void onSuccess(PendingDynamicLinkData pendingDynamicLinkData) {

                        Uri deepLink = null;
                        if (pendingDynamicLinkData != null) {
                            deepLink = pendingDynamicLinkData.getLink();

                            _logger.debug("Deep link is : " + deepLink.toString());

                            MessageBridge bridge = MessageBridge.getInstance();
                            bridge.callCpp(k__onReceiveDynamicLink, deepLink.toString());
                        }

                    }
                })
                .addOnFailureListener(_activity, new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {

                    }
                });
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FirebaseDynamicLink";
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

    }

    @Override
    public void destroy() {

    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }
}
