package com.ee.plugin;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.util.Log;

import com.crashlytics.android.Crashlytics;
import com.crashlytics.android.core.BuildConfig;
import com.crashlytics.android.core.CrashTest;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.ee.core.PluginProtocol;

import java.util.Map;

import io.fabric.sdk.android.Fabric;

/**
 * Created by Zinge on 7/4/16.
 */
public class CrashlyticsProtocol extends PluginProtocol {
    public CrashlyticsProtocol(Context context) {
        super(context);
        Fabric.Builder builder = new Fabric.Builder(context)
            .debuggable(BuildConfig.DEBUG)
            .logger(new CrashlyticsLogger(BuildConfig.DEBUG ? Log.DEBUG : Log.INFO))
            .kits(new Crashlytics(), new CrashlyticsNdk());

        Fabric.with(builder.build());
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

    public void log(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 3);

        Integer priority = (Integer) dict.get("priority");
        String tag = (String) dict.get("tag");
        String message = (String) dict.get("message");

        assert (priority != null);
        assert (tag != null);
        assert (message != null);

        Crashlytics.log(priority, tag, message);
    }

    public void setString(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 2);

        String key = (String) dict.get("key");
        String value = (String) dict.get("value");

        assert (key != null);
        assert (value != null);

        Crashlytics.setString(key, value);
    }

    public void setBool(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 2);

        String key = (String) dict.get("key");
        Boolean value = (Boolean) dict.get("value");

        assert (key != null);
        assert (value != null);

        Crashlytics.setBool(key, value);
    }

    public void setInt(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 2);

        String key = (String) dict.get("key");
        Integer value = (Integer) dict.get("value");

        assert (key != null);
        assert (value != null);

        Crashlytics.setInt(key, value);
    }

    public void setUserIdentifier(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 1);

        String identifier = (String) dict.get("identifier");

        assert (identifier != null);

        Crashlytics.setUserIdentifier(identifier);
    }

    public void setUserName(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 1);

        String name = (String) dict.get("name");

        assert (name != null);

        Crashlytics.setUserName(name);
    }

    public void setUserEmail(@NonNull Map<String, Object> dict) {
        assert (dict.size() == 1);

        String email = (String) dict.get("email");

        assert (email != null);

        Crashlytics.setUserName(email);
    }
}
