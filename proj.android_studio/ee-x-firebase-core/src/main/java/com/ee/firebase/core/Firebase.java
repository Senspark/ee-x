package com.ee.firebase.core;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import androidx.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;

/**
 * Created by Zinge on 7/4/16.
 */
public class Firebase implements PluginProtocol {
    private static final Logger _logger = new Logger(Firebase.class.getName());

    private native void setActivity(Object activity);

    public Firebase() {
        _logger.debug("constructor begin.");
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
