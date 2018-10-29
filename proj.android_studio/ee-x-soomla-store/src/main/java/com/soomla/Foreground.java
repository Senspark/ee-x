package com.soomla;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.Application;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;

import com.soomla.events.AppToBackgroundEvent;
import com.soomla.events.AppToForegroundEvent;

import java.util.List;

/**
 * This implementation is based on the great article by Steve Liles:
 * http://steveliles.github.io/is_my_android_app_currently_foreground_or_background.html
 *
 * Thanks Steve!
 */


/**
 * This Class Helps the app determine if it's running on the background or not.
 *
 * Special Cases:
 * 1. Changing Orientation - Always report that the app is at the foreground.
 * 2. Incoming Call - Report background. once the call ended, report foreground.
 * 3. Store purchases (Amazon, Google play, etc...) - Always report that the app is at the foreground.
 * 4. Pressable Notification - Report background, once the user clicks "ok" report foreground.
 *
 */
public abstract class Foreground {

    private static Foreground instance;

    public static final String TAG = "SOOMLA " + Foreground.class.getName();

    // used to know if there's an outside operation running and the app will return to foreground soon.
    public boolean OutsideOperation = false; // deprecated

    // Orientation change flag
    boolean isChangingOrientation = false;

    /**
     * Initializes Foreground
     *
     * @return an initialised Foreground instance
     */
    public static Foreground init() {
        if (instance == null) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH) {
                instance = new ForegroundNew();
            } else {
                instance = new ForegroundOld();
                SoomlaUtils.LogDebug(TAG, "Tried to instantiate ForegroundOld");
            }
        }
        return instance;
    }

    public abstract boolean isForeground();
    public abstract boolean isBackground();

    private static class ForegroundOld extends Foreground {

        @Override
        public boolean isForeground() {
            ActivityManager activityManager = (ActivityManager) SoomlaApp.getAppContext().getSystemService(Context.ACTIVITY_SERVICE);
            List<ActivityManager.RunningAppProcessInfo> appProcesses = activityManager.getRunningAppProcesses();
            if (appProcesses == null) {
                return false;
            }
            final String packageName = SoomlaApp.getAppContext().getPackageName();
            for (ActivityManager.RunningAppProcessInfo appProcess : appProcesses) {
                if (appProcess.importance == ActivityManager.RunningAppProcessInfo.IMPORTANCE_FOREGROUND && appProcess.processName.equals(packageName)) {
                    return true;
                }
            }
            return false;
        }

        @Override
        public boolean isBackground() {
            return !isForeground();
        }

    }

    @TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
    private static class ForegroundNew extends Foreground implements Application.ActivityLifecycleCallbacks {

        private int count = 0;
        private int refs = 0;

        // Constructor for inheriting class
        public ForegroundNew() {
            SoomlaApp.appInstance().registerActivityLifecycleCallbacks(this);
        }

        @Override
        public boolean isForeground() {
            return refs > 0;
        }

        @Override
        public boolean isBackground() {
            return refs == 0;
        }

        @Override
        public void onActivityResumed(Activity activity) {
            if (count <= 0) {
                count = 1;
            }
            if (refs <= 0) {
                refs = 1;
            }
        }

        @Override
        public void onActivityPaused(Activity activity) {
            if (count <= 0) {
                count = 1;
            }
            if (refs <= 0) {
                refs = 1;
            }
        }

        @Override
        public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
            count++;
        }

        @Override
        public void onActivityStarted(Activity activity) {

            // Escaping Unity Bug on 'proxy' activity not being stopped
            if (!activity.getLocalClassName()
                    .equals("com.unity3d.player.UnityPlayerProxyActivity")) {
                // Checking the number of refs to determine the state we were at
                if (++refs == 1 && !isChangingOrientation) {
                    // Passed to foreground, so posting event
                    BusProvider.getInstance().post(new AppToForegroundEvent());
                    SoomlaUtils.LogDebug(TAG, "became foreground");
                } else {
                    SoomlaUtils.LogDebug(TAG, "still foreground");
                }
            }
        }


        @Override
        public void onActivityStopped(Activity activity) {

            isChangingOrientation = activity.isChangingConfigurations();

            // Escaping Unity Bug on 'proxy' activity not being stopped
            if (!activity.getLocalClassName()
                    .equals("com.unity3d.player.UnityPlayerProxyActivity")) {
                // Checking if we're backgrounded
                if (--refs == 0 && !isChangingOrientation) {
                    // app went to background - posting event
                    BusProvider.getInstance().post(new AppToBackgroundEvent());
                    SoomlaUtils.LogDebug(TAG, "became background");
                } else {
                    SoomlaUtils.LogDebug(TAG, "still foreground");
                }
            }
        }

        @Override
        public void onActivitySaveInstanceState(Activity activity, Bundle outState) {
        }

        @Override
        public void onActivityDestroyed(Activity activity) {

            count--;
            if (count == 0 && isForeground()) {
                SoomlaUtils.LogDebug(TAG, "destroyed weirdly");
                BusProvider.getInstance().post(new AppToBackgroundEvent());
            }
        }
    }
}
