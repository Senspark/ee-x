package com.ee.core.internal;

import android.content.res.Resources;
import android.support.annotation.NonNull;
import android.util.DisplayMetrics;

/**
 * Created by Zinge on 10/6/17.
 */

public class Metrics {
    private static final String k__getDensity = "Metrics_getDensity";

    public static void registerHandlers() {
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

    public static int getDensityDpi() {
        DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();
        return metrics.densityDpi;
    }

    /// https://stackoverflow.com/questions/4605527/converting-pixels-to-dp
    public static double convertDpToPixel(double dp) {
        return dp * getDensity();
    }
}
