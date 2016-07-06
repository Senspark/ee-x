package com.ee.plugin;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.core.CrashTest;
import com.ee.core.PluginProtocol;

import java.util.Map;

/**
 * Created by Zinge on 7/4/16.
 */
public class CrashlyticsProtocol extends PluginProtocol {
    public CrashlyticsProtocol(Context context) {
        super(context);
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "CrashlyticsProtocol";
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
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
    public boolean onBackPressed() {
        return false;
    }

    public void causeCrash() {
        Crashlytics.getInstance().crash();
    }

    public void causeException() {
        (new CrashTest()).throwRuntimeException("This is a test exception!");
    }

    private void _log(@NonNull Integer priority, @NonNull String tag, @NonNull String message) {
        Crashlytics.log(priority, tag, message);
    }

    public void log(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 3);

        Integer priority = (Integer) dict.get("priority");
        String tag = (String) dict.get("tag");
        String message = (String) dict.get("message");

        _log(priority, tag, message);
    }
}
