package com.ee.firebase.crashlytics;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.IPlugin;
import com.ee.core.internal.Utils;

public class FirebaseCrashlytics implements IPlugin {
    private static final String kLog = "FirebaseCrashlytics_log";

    private static final Logger _logger = new Logger(FirebaseCrashlytics.class.getName());

    private IMessageBridge _bridge;

    public FirebaseCrashlytics(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Utils.checkMainThread();
        _bridge = bridge;
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FirebaseCrashlytics";
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
        Utils.checkMainThread();
        deregisterHandlers();
        _bridge = null;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _bridge.registerHandler(kLog, message -> {
            log(message);
            return "";
        });
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _bridge.deregisterHandler(kLog);
    }

    public void log(@NonNull String message) {
        com.google.firebase.crashlytics.FirebaseCrashlytics.getInstance().log(message);
    }
}
