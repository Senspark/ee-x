package com.ee.firebase.performance;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.JsonUtils;
import com.google.firebase.perf.metrics.Trace;

import java.util.Map;

public class FirebasePerformanceTrace {
    private static final String k__key = "key";
    private static final String k__value = "value";

    private IMessageBridge _bridge;
    private String _traceName;
    private Trace _trace;

    public FirebasePerformanceTrace(@NonNull IMessageBridge bridge,
                                    @NonNull Trace trace,
                                    @NonNull String traceName) {
        _bridge = bridge;
        _traceName = traceName;
        _trace = trace;
        registerHandlers();
    }

    void destroy() {
        deregisterHandlers();
        _bridge = null;
        _trace = null;
        _traceName = null;
    }

    @NonNull
    private String k__start() {
        return "FirebasePerformance_start_" + _traceName;
    }

    @NonNull
    private String k__stop() {
        return "FirebasePerformance_stop_" + _traceName;
    }

    @NonNull
    String k__incrementMetric() {
        return "FirebasePerformance_incrementMetric_" + _traceName;
    }

    @NonNull
    String k__getLongMetric() {
        return "FirebasePerformance_getLongMetric_" + _traceName;
    }

    @NonNull
    String k__putMetric() {
        return "FirebasePerformance_putMetric_" + _traceName;
    }

    private void registerHandlers() {
        _bridge.registerHandler(message -> {
            start();
            return "";
        }, k__start());

        _bridge.registerHandler(message -> {
            stop();
            return "";
        }, k__stop());

        _bridge.registerHandler(message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assert dict != null;

            String key = (String) dict.get(k__key);
            Integer value = (Integer) dict.get(k__value);
            incrementMetric(key, value);
            return "";
        }, k__incrementMetric());

        _bridge.registerHandler(message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assert dict != null;

            String key = (String) dict.get(k__key);
            Long value = (Long) dict.get(k__value);
            putMetric(key, value);
            return "";
        }, k__putMetric());

        _bridge.registerHandler(message -> {
            String metricName = message;
            return String.valueOf(getLongMetric(metricName));
        }, k__getLongMetric());
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__start());
        _bridge.deregisterHandler(k__stop());
        _bridge.deregisterHandler(k__incrementMetric());
        _bridge.deregisterHandler(k__putMetric());
        _bridge.deregisterHandler(k__getLongMetric());
    }

    public void start() {
        _trace.start();
    }

    public void stop() {
        _trace.stop();
    }

    public void incrementMetric(@NonNull String traceName, long value) {
        _trace.incrementMetric(traceName, value);
    }

    public void putMetric(@NonNull String name, long value) {
        _trace.putMetric(name, value);
    }

    public long getLongMetric(@NonNull String name) {
        return _trace.getLongMetric(name);
    }
}
