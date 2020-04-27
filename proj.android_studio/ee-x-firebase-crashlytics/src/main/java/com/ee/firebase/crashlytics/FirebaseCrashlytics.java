package com.ee.firebase.crashlytics;

import android.app.Activity;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;

public class FirebaseCrashlytics implements PluginProtocol {
    private static final String kLog = "FirebaseCrashlytics_log";

    private static final Logger _logger = new Logger(FirebaseCrashlytics.class.getName());

    private IMessageBridge _bridge;

    public FirebaseCrashlytics() {
        Utils.checkMainThread();
        _bridge = MessageBridge.getInstance();
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FirebaseCrashlytics";
    }

    @Override
    public void onCreate(@NonNull final Activity activity) {
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
        Utils.checkMainThread();
        deregisterHandlers();
    }

    @Override
    public boolean onActivityResult(final int requestCode, final int responseCode, final Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _bridge.registerHandler(message -> {
            log(message);
            return "";
        }, kLog);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _bridge.deregisterHandler(kLog);
    }

    public void log(@NonNull String message) {
        com.google.firebase.crashlytics.FirebaseCrashlytics.getInstance().log(message);
    }
}
