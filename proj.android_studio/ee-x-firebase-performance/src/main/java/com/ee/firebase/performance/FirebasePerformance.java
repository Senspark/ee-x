package com.ee.firebase.performance;

import android.app.Activity;
import android.content.Intent;
import android.support.annotation.NonNull;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;
import com.google.firebase.perf.metrics.Trace;
import java.util.HashMap;
import java.util.Map;

public class FirebasePerformance implements PluginProtocol {
    private static final String k__setDataCollectionEnabled = "FirebasePerformance_setDataCollectionEnabled";
    private static final String k__isDataCollectionEnabled  = "FirebasePerformance_isDataCollectionEnabled";
    private static final String k__startTrace               = "FirebasePerformance_startTrace";
    private static final String k__newTrace                 = "FirebasePerformance_newTrace";

    private static final Logger _logger = new Logger(FirebasePerformance.class.getName());

    private com.google.firebase.perf.FirebasePerformance    _performance;
    private Map<String, FirebasePerformanceTrace>           _traces;

    public FirebasePerformance() {
        Utils.checkMainThread();
        _performance = com.google.firebase.perf.FirebasePerformance.getInstance();
        _traces = new HashMap<>();
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FirebasePerformance";
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
        _traces.clear();
        _traces = null;
        _performance = null;
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
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                setDataCollectionEnabled(Utils.toBoolean(message));
                return "";
            }
        }, k__setDataCollectionEnabled);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                return Utils.toString(isDataCollectionEnabled());
            }
        }, k__isDataCollectionEnabled);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                String traceName = message;
                return Utils.toString(startTrace(traceName));
            }
        }, k__startTrace);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                String traceName = message;
                return Utils.toString(newTrace(traceName));
            }
        }, k__newTrace);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__setDataCollectionEnabled);
        bridge.deregisterHandler(k__isDataCollectionEnabled);
        bridge.deregisterHandler(k__newTrace);
        bridge.deregisterHandler(k__startTrace);
    }

    public void setDataCollectionEnabled(boolean enabled) {
        _performance.setPerformanceCollectionEnabled(enabled);
    }

    public boolean isDataCollectionEnabled() {
        return _performance.isPerformanceCollectionEnabled();
    }

    public boolean startTrace(@NonNull String traceName) {
        if (_traces.containsKey(traceName)) {
            return false;
        }
        Trace trace = _performance.startTrace(traceName);
        _traces.put(traceName, new FirebasePerformanceTrace(trace, traceName));
        return true;
    }

    public boolean newTrace(@NonNull String traceName) {
        if (_traces.containsKey(traceName)) {
            return false;
        }
        Trace trace = _performance.newTrace(traceName);
        _traces.put(traceName, new FirebasePerformanceTrace(trace, traceName));
        return true;
    }
}
