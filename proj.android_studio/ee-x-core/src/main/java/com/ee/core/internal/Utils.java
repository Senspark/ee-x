package com.ee.core.internal;

import android.app.Activity;
import android.widget.FrameLayout;

/**
 * Created by Zinge on 10/9/17.
 */

public class Utils {
    public static FrameLayout getRootView(Activity activity) {
        return (FrameLayout) activity.findViewById(android.R.id.content).getRootView();
    }
}
