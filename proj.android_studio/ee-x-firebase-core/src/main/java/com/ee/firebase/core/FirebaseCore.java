package com.ee.firebase.core;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.IPlugin;
import com.ee.core.Logger;
import com.google.firebase.FirebaseApp;

/**
 * Created by Zinge on 7/4/16.
 */
public class FirebaseCore implements IPlugin {
    private static final Logger _logger = new Logger(FirebaseCore.class.getName());

    public FirebaseCore(@NonNull Context context, @NonNull IMessageBridge bridge) {
        _logger.debug("constructor begin.");
        FirebaseApp.initializeApp(context);
        _logger.debug("constructor end.");
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
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
}
