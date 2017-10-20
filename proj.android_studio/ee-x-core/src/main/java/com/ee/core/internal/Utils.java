package com.ee.core.internal;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.widget.FrameLayout;

import com.ee.core.Logger;

/**
 * Created by Zinge on 10/9/17.
 */

public class Utils {
    private static final Logger _logger = new Logger(Utils.class.getName());

    public static FrameLayout getRootView(Activity activity) {
        return (FrameLayout) activity.findViewById(android.R.id.content).getRootView();
    }

    private static native void signal();

    @SuppressWarnings("unused")
    public static void runOnUiThread() {
        if (isMainThread()) {
            signal();
        } else {
            Handler handler = new Handler(Looper.getMainLooper());
            boolean result = handler.post(new Runnable() {
                @Override
                public void run() {
                    signal();
                }
            });
            if (!result) {
                _logger.error("runOnUiThread: failed to post the runnable");
            }
        }
    }

    public static void checkMainThread() {
        if (!isMainThread()) {
            _logger.error("Current thread is not the main thread");
            assert false;
        }
    }

    public static boolean isMainThread() {
        return Thread.currentThread() == Looper.getMainLooper().getThread();
    }

    @NonNull
    public static String toString(boolean value) {
        return value ? "true" : "false";
    }

    @NonNull
    public static Boolean toBoolean(@NonNull String value) {
        assert value.equals("true") || value.equals("false");
        return value.equals("true");
    }

    /// https://stackoverflow.com/questions/1069066/get-current-stack-trace-in-java
    @NonNull
    public static StackTraceElement[] getCurrentStackTrace() {
        return Thread.currentThread().getStackTrace();
    }
}
