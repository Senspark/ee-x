package com.ee.firebase.performance;

import androidx.annotation.NonNull;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.JsonUtils;
import com.google.firebase.perf.metrics.Trace;
import java.util.Map;

public class FirebasePerformanceTrace {
    private static final String k__key      = "key";
    private static final String k__value    = "value";

    private String _traceName;
    private Trace _trace;

    public FirebasePerformanceTrace(Trace trace, String traceName) {
        _traceName = traceName;
        _trace = trace;
        registerHandlers();
    }

    void destroy() {
        deregisterHandlers();
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

    @NonNull String k__incrementMetric() {
        return "FirebasePerformance_incrementMetric_" + _traceName;
    }

    @NonNull String k__getLongMetric() {
        return "FirebasePerformance_getLongMetric_" + _traceName;
    }

    @NonNull String k__putMetric() {
        return "FirebasePerformance_putMetric_" + _traceName;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                start();
                return "";
            }
        }, k__start());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                stop();
                return "";
            }
        }, k__stop());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;

                String key = (String) dict.get(k__key);
                Integer value = (Integer) dict.get(k__value);
                incrementMetric(key, value);
                return "";
            }
        }, k__incrementMetric());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert  dict != null;

                String key = (String) dict.get(k__key);
                Long value = (Long) dict.get(k__value);
                putMetric(key, value);
                return "";
            }
        }, k__putMetric());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull final String message) {
                String metricName = message;
                return String.valueOf(getLongMetric(metricName));
            }
        }, k__getLongMetric());
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__start());
        bridge.deregisterHandler(k__stop());
        bridge.deregisterHandler(k__incrementMetric());
        bridge.deregisterHandler(k__putMetric());
        bridge.deregisterHandler(k__getLongMetric());
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
