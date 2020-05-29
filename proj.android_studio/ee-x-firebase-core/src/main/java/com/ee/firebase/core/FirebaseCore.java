package com.ee.firebase.core;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.IPlugin;
import com.ee.core.Logger;
import com.ee.core.PluginManager;
import com.google.firebase.FirebaseApp;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 7/4/16.
 */
public class FirebaseCore implements IPlugin {
    private static final Logger _logger = new Logger(FirebaseCore.class.getName());
    private static final Object _activityMutex = new Object();

    private static Object staticGetActivity() {
        // Called from C++.
        PluginManager manager = PluginManager.getInstance();
        FirebaseCore plugin = (FirebaseCore) manager.getPlugin("FirebaseCore");
        assertThat(plugin).isNotNull();
        synchronized (_activityMutex) {
            return plugin._activity;
        }
    }

    private Activity _activity;

    public FirebaseCore(@NonNull Context context, @NonNull IMessageBridge bridge) {
        _logger.debug("constructor begin.");
        FirebaseApp.initializeApp(context);
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FirebaseCore";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        synchronized (_activityMutex) {
            _activity = activity;
        }
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
        synchronized (_activityMutex) {
            _activity = null;
        }
    }

    @Override
    public void destroy() {
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }
}
