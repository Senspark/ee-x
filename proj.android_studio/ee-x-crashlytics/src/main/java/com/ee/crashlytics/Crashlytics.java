package com.ee.crashlytics;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.crashlytics.android.answers.Answers;
import com.crashlytics.android.answers.AnswersEvent;
import com.crashlytics.android.answers.CustomEvent;
import com.crashlytics.android.answers.InviteEvent;
import com.crashlytics.android.answers.LevelEndEvent;
import com.crashlytics.android.answers.LevelStartEvent;
import com.crashlytics.android.answers.PurchaseEvent;
import com.crashlytics.android.core.CrashTest;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;

import java.math.BigDecimal;
import java.util.Currency;
import java.util.Map;

import io.fabric.sdk.android.Fabric;

/**
 * Created by Zinge on 7/4/16.
 */
public class Crashlytics implements PluginProtocol {
    private static final String k__crashlytics_cause_crash     = "__crashlytics_cause_crash";
    private static final String k__crashlytics_cause_exception = "__crashlytics_cause_exception";
    private static final String k__crashlytics_set_log_level   = "__crashlytics_set_log_level";
    private static final String k__crashlytics_log             = "__crashlytics_log";

    private static final Logger _logger = new Logger(Crashlytics.class.getName());

    public Crashlytics(Context context) {
        _logger.debug("constructor begin: context = " + context);

        registerHandlers();

        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Crashlytics";
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
        deregisterHandlers();
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                causeCrash();
                return "";
            }
        }, k__crashlytics_cause_crash);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                causeException();
                return "";
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
                return "";
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
                return "";
            }
        }, k__crashlytics_log);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__crashlytics_cause_crash);
        bridge.deregisterHandler(k__crashlytics_cause_exception);
        bridge.deregisterHandler(k__crashlytics_log);
        bridge.deregisterHandler(k__crashlytics_set_log_level);
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
}
