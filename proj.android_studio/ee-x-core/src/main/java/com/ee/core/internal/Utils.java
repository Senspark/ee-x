package com.ee.core.internal;

import android.app.Activity;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
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
        if (Looper.myLooper() == Looper.getMainLooper()) {
            signal();
        } else {
            Handler handler = new Handler(Looper.getMainLooper());
            handler.post(new Runnable() {
                @Override
                public void run() {
                    signal();
                }
            });
        }
    }
}
