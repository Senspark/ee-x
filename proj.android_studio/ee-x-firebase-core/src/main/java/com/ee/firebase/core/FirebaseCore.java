package com.ee.firebase.core;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.google.firebase.FirebaseApp;

/**
 * Created by Zinge on 7/4/16.
 */
public class FirebaseCore implements PluginProtocol {
    private static final Logger _logger = new Logger(FirebaseCore.class.getName());

    private native void setActivity(Object activity);

    public FirebaseCore(@NonNull Context context, @NonNull IMessageBridge bridge) {
        _logger.debug("constructor begin.");
        FirebaseApp.initializeApp(context);
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Firebase";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        setActivity(activity);
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
