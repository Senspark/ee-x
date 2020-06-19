package com.ee.firebase.performance;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.IPlugin;
import com.ee.core.Logger;
import com.ee.core.internal.Thread;
import com.ee.core.internal.Utils;
import com.google.firebase.perf.metrics.Trace;

import java.util.HashMap;
import java.util.Map;

public class FirebasePerformance implements IPlugin {
    private static final String k__setDataCollectionEnabled = "FirebasePerformance_setDataCollectionEnabled";
    private static final String k__isDataCollectionEnabled = "FirebasePerformance_isDataCollectionEnabled";
    private static final String k__startTrace = "FirebasePerformance_startTrace";
    private static final String k__newTrace = "FirebasePerformance_newTrace";

    private static final Logger _logger = new Logger(FirebasePerformance.class.getName());

    private IMessageBridge _bridge;
    private com.google.firebase.perf.FirebasePerformance _performance;
    private Map<String, FirebasePerformanceTrace> _traces;

    public FirebasePerformance(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Thread.checkMainThread();
        _bridge = bridge;
        _performance = com.google.firebase.perf.FirebasePerformance.getInstance();
        _traces = new HashMap<>();
        registerHandlers();
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
        Thread.checkMainThread();
        deregisterHandlers();
        for (String key : _traces.keySet()) {
            _traces.get(key).destroy();
        }
        _traces.clear();
        _traces = null;
        _performance = null;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        Thread.checkMainThread();
        _bridge.registerHandler(k__setDataCollectionEnabled, message -> {
            setDataCollectionEnabled(Utils.toBoolean(message));
            return "";
        });

        _bridge.registerHandler(k__isDataCollectionEnabled, message ->
            Utils.toString(isDataCollectionEnabled()));

        _bridge.registerHandler(k__startTrace, message -> {
            String traceName = message;
            return Utils.toString(startTrace(traceName));
        });

        _bridge.registerHandler(k__newTrace, message -> {
            String traceName = message;
            return Utils.toString(newTrace(traceName));
        });
    }

    private void deregisterHandlers() {
        Thread.checkMainThread();
        _bridge.deregisterHandler(k__setDataCollectionEnabled);
        _bridge.deregisterHandler(k__isDataCollectionEnabled);
        _bridge.deregisterHandler(k__newTrace);
        _bridge.deregisterHandler(k__startTrace);
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
        _traces.put(traceName, new FirebasePerformanceTrace(_bridge, trace, traceName));
        return true;
    }

    public boolean newTrace(@NonNull String traceName) {
        if (_traces.containsKey(traceName)) {
            return false;
        }
        Trace trace = _performance.newTrace(traceName);
        _traces.put(traceName, new FirebasePerformanceTrace(_bridge, trace, traceName));
        return true;
    }
}
