package com.ee.core;

import android.content.res.Resources;
import android.support.annotation.NonNull;
import android.util.DisplayMetrics;

import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

/**
 * Created by Zinge on 10/6/17.
 */

public class Metrics {
    private static final String k__getDensity = "Metrics_getDensity";

    public static void initialize() {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                double density = getDensity();
                return String.valueOf(density);
            }
        }, k__getDensity);
    }

    @SuppressWarnings("WeakerAccess")
    public static double getDensity() {
        DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();
        return metrics.density;
    }
}
