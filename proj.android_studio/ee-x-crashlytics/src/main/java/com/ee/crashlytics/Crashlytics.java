package com.ee.crashlytics;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.util.Log;

import com.crashlytics.android.core.CrashTest;
import com.crashlytics.android.ndk.CrashlyticsNdk;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.DictionaryUtils;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

import java.util.Map;

import io.fabric.sdk.android.Fabric;

/**
 * Created by Zinge on 7/4/16.
 */
public class Crashlytics implements PluginProtocol {
    private static final String k__crashlytics_cause_crash         = "__crashlytics_cause_crash";
    private static final String k__crashlytics_cause_exception     =
        "__crashlytics_cause_exception";
    private static final String k__crashlytics_set_log_level       = "__crashlytics_set_log_level";
    private static final String k__crashlytics_log                 = "__crashlytics_log";
    private static final String k__crashlytics_set_string          = "__crashlytics_set_string";
    private static final String k__crashlytics_set_bool            = "__crashlytics_set_bool";
    private static final String k__crashlytics_set_int             = "__crashlytics_set_int";
    private static final String k__crashlytics_set_user_identifier =
        "__crashlytics_set_user_identifier";
    private static final String k__crashlytics_set_user_name       = "__crashlytics_set_user_name";
    private static final String k__crashlytics_set_user_email      = "__crashlytics_set_user_email";

    private static final Logger _logger = new Logger(Crashlytics.class.getName());

    public Crashlytics(Context context) {
        _logger.debug("constructor begin: context = " + context);

        Fabric.Builder builder = new Fabric.Builder(context)
            .logger(new CrashlyticsLogger(Log.DEBUG))
            .kits(new com.crashlytics.android.Crashlytics(), new CrashlyticsNdk());

        Fabric.with(builder.build());

        registerHandlers();

        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Crashlytics";
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
        deregisterHandlers();
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                causeCrash();
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_cause_crash);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                causeException();
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_cause_exception);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                Integer priority = (Integer) dict.get("priority");

                setLogLevel(priority);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_log_level);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                Integer priority = (Integer) dict.get("priority");
                String tag = (String) dict.get("tag");
                String message = (String) dict.get("message");

                log(priority, tag, message);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_log);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String key = (String) dict.get("key");
                String value = (String) dict.get("value");

                setString(key, value);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_string);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String key = (String) dict.get("key");
                Boolean value = (Boolean) dict.get("value");

                setBool(key, value);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_bool);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String key = (String) dict.get("key");
                Integer value = (Integer) dict.get("value");

                setInt(key, value);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_int);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                setUserIdentifier(msg);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_user_identifier);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                setUserName(msg);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_user_name);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                setUserEmail(msg);
                return DictionaryUtils.emptyResult();
            }
        }, k__crashlytics_set_user_email);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__crashlytics_cause_crash);
        bridge.deregisterHandler(k__crashlytics_cause_exception);
        bridge.deregisterHandler(k__crashlytics_log);
        bridge.deregisterHandler(k__crashlytics_set_log_level);
        bridge.deregisterHandler(k__crashlytics_set_string);
        bridge.deregisterHandler(k__crashlytics_set_bool);
        bridge.deregisterHandler(k__crashlytics_set_int);
        bridge.deregisterHandler(k__crashlytics_set_user_identifier);
        bridge.deregisterHandler(k__crashlytics_set_user_name);
        bridge.deregisterHandler(k__crashlytics_set_user_email);
    }

    @SuppressWarnings("WeakerAccess")
    public void causeCrash() {
        com.crashlytics.android.Crashlytics.getInstance().crash();
    }

    @SuppressWarnings("WeakerAccess")
    public void causeException() {
        (new CrashTest()).throwRuntimeException("This is a test exception!");
    }

    @SuppressWarnings("WeakerAccess")
    public void setLogLevel(@NonNull Integer priority) {
        Fabric.getLogger().setLogLevel(priority);
    }

    public void log(@NonNull Integer priority, @NonNull String tag, @NonNull String message) {
        com.crashlytics.android.Crashlytics.log(priority, tag, message);
    }

    @SuppressWarnings("WeakerAccess")
    public void setString(@NonNull String key, @NonNull String value) {
        com.crashlytics.android.Crashlytics.setString(key, value);
    }

    @SuppressWarnings("WeakerAccess")
    public void setBool(@NonNull String key, @NonNull Boolean value) {
        com.crashlytics.android.Crashlytics.setBool(key, value);
    }

    @SuppressWarnings("WeakerAccess")
    public void setInt(@NonNull String key, @NonNull Integer value) {
        com.crashlytics.android.Crashlytics.setInt(key, value);
    }

    @SuppressWarnings("WeakerAccess")
    public void setUserIdentifier(@NonNull String identifier) {
        com.crashlytics.android.Crashlytics.setUserIdentifier(identifier);
    }

    @SuppressWarnings("WeakerAccess")
    public void setUserName(@NonNull String name) {
        com.crashlytics.android.Crashlytics.setUserName(name);
    }

    @SuppressWarnings("WeakerAccess")
    public void setUserEmail(@NonNull String email) {
        com.crashlytics.android.Crashlytics.setUserName(email);
    }
}
