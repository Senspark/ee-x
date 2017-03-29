package com.ee.crashlytics;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.util.Log;

import com.crashlytics.android.core.CrashTest;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;

import java.util.Map;

import io.fabric.sdk.android.Fabric;

/**
 * Created by Zinge on 7/4/16.
 */
public class Crashlytics extends PluginProtocol {
    public Crashlytics(Context context) {
        super(context);

        Fabric.Builder builder = new Fabric.Builder(context)
            .logger(new CrashlyticsLogger(Log.DEBUG))
            .kits(new com.crashlytics.android.Crashlytics(), new CrashlyticsNdk());

        Fabric.with(builder.build());
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
        com.crashlytics.android.Crashlytics.getInstance().crash();
    }

    public void causeException() {
        (new CrashTest()).throwRuntimeException("This is a test exception!");
    }

    public void setDebuggable(boolean debuggable) {
        super.setDebuggable(debuggable);
        if (debuggable) {
            Fabric.getLogger().setLogLevel(Log.DEBUG);
        } else {
            Fabric.getLogger().setLogLevel(Log.INFO);
        }
    }

    public void log(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 4);

        Integer priority = (Integer) dict.get("priority");
        String tag = (String) dict.get("tag");
        String message = (String) dict.get("message");

        assert (priority != null);
        assert (tag != null);
        assert (message != null);

        com.crashlytics.android.Crashlytics.log(priority, tag, message);
    }

    public void setString(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 2);

        String key = (String) dict.get("key");
        String value = (String) dict.get("value");

        assert (key != null);
        assert (value != null);

        com.crashlytics.android.Crashlytics.setString(key, value);
    }

    public void setBool(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 2);

        String key = (String) dict.get("key");
        Boolean value = (Boolean) dict.get("value");

        assert (key != null);
        assert (value != null);

        com.crashlytics.android.Crashlytics.setBool(key, value);
    }

    public void setInt(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 2);

        String key = (String) dict.get("key");
        Integer value = (Integer) dict.get("value");

        assert (key != null);
        assert (value != null);

        com.crashlytics.android.Crashlytics.setInt(key, value);
    }

    public void setUserIdentifier(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 1);

        String identifier = (String) dict.get("identifier");

        assert (identifier != null);

        com.crashlytics.android.Crashlytics.setUserIdentifier(identifier);
    }

    public void setUserName(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 1);

        String name = (String) dict.get("name");

        assert (name != null);

        com.crashlytics.android.Crashlytics.setUserName(name);
    }

    public void setUserEmail(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 1);

        String email = (String) dict.get("email");

        assert (email != null);

        com.crashlytics.android.Crashlytics.setUserName(email);
    }
}
