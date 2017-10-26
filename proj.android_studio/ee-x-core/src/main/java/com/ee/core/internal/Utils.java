package com.ee.core.internal;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.NonNull;
import android.util.DisplayMetrics;
import android.widget.FrameLayout;

import com.ee.core.Logger;
import com.ee.core.PluginManager;

import java.security.MessageDigest;
import java.util.Locale;

/**
 * Created by Zinge on 10/9/17.
 */

public class Utils {
    public static FrameLayout getRootView(Activity activity) {
        return (FrameLayout) activity.findViewById(android.R.id.content).getRootView();
    }

    public static void checkMainThread() {
        if (!isMainThread()) {
            _logger.error("Current thread is not the main thread");
            assert false;
        }
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

    private static final Logger _logger = new Logger(Utils.class.getName());

    private static final String k__isMainThread                  = "Utils_isMainThread";
    private static final String k__runOnUiThread                 = "Utils_runOnUiThread";
    private static final String k__runOnUiThreadCallback         = "Utils_runOnUiThreadCallback";
    private static final String k__getSHA1CertificateFingerprint =
        "Utils_getSHA1CertificateFingerprint";
    private static final String k__getVersionName                = "Utils_getVersionName";
    private static final String k__getVersionCode                = "Utils_getVersionCode";
    private static final String k__isApplicationInstalled        = "Utils_isApplicationInstalled";
    private static final String k__openApplication               = "Utils_openApplication";
    private static final String k__isTablet                      = "Utils_isTablet";
    private static final String k__testConnection                = "Utils_testConnection";

    public static void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isMainThread());
            }
        }, k__isMainThread);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(Utils.runOnUiThread());
            }
        }, k__runOnUiThread);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Context context = PluginManager.getInstance().getContext();
                return getSHA1CertificateFingerprint(context);
            }
        }, k__getSHA1CertificateFingerprint);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Context context = PluginManager.getInstance().getContext();
                return getVersionName(context);
            }
        }, k__getVersionName);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Context context = PluginManager.getInstance().getContext();
                return String.valueOf(getVersionCode(context));
            }
        }, k__getVersionCode);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Context context = PluginManager.getInstance().getContext();
                String applicationId = message;
                return Utils.toString(isApplicationInstalled(context, applicationId));
            }
        }, k__isApplicationInstalled);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Context context = PluginManager.getInstance().getContext();
                String applicationId = message;
                return Utils.toString(openApplication(context, applicationId));
            }
        }, k__openApplication);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isTablet());
            }
        }, k__isTablet);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Context context = PluginManager.getInstance().getContext();
                return Utils.toString(testConnection(context));
            }
        }, k__testConnection);
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean isMainThread() {
        return Thread.currentThread() == Looper.getMainLooper().getThread();
    }

    private static void signalMainThread() {
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__runOnUiThreadCallback);
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean runOnUiThread() {
        if (isMainThread()) {
            signalMainThread();
            return true;
        }
        Handler handler = new Handler(Looper.getMainLooper());
        boolean result = handler.post(new Runnable() {
            @Override
            public void run() {
                signalMainThread();
            }
        });
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable");
        }
        return false;
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public static String getSHA1CertificateFingerprint(@NonNull Context context) {
        String hash = "";
        try {
            PackageManager packetManager = context.getPackageManager();
            int flags = PackageManager.GET_SIGNATURES;
            PackageInfo info = packetManager.getPackageInfo(context.getPackageName(), flags);
            MessageDigest digest = MessageDigest.getInstance("SHA1");
            digest.update(info.signatures[0].toByteArray());

            StringBuilder builder = new StringBuilder();
            for (byte b : digest.digest()) {
                String hex = Integer.toString(b & 0xff, 16);
                if (hex.length() == 1) {
                    builder.append('0');
                }
                builder.append(hex.toUpperCase(Locale.US));
            }
            hash = builder.toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return hash;
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public static String getVersionName(@NonNull Context context) {
        PackageManager packetManager = context.getPackageManager();
        String versionName = "";
        try {
            PackageInfo info = packetManager.getPackageInfo(context.getPackageName(), 0);
            versionName = info.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return versionName;
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public static int getVersionCode(@NonNull Context context) {
        PackageManager packetManager = context.getPackageManager();
        int versionCode = -1;
        try {
            PackageInfo info = packetManager.getPackageInfo(context.getPackageName(), 0);
            versionCode = info.versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return versionCode;
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean isApplicationInstalled(@NonNull Context context,
                                                 @NonNull String applicationId) {
        PackageManager packetManager = context.getPackageManager();
        boolean installed = false;
        try {
            packetManager.getPackageInfo(applicationId, PackageManager.GET_ACTIVITIES);
            installed = true;
        } catch (PackageManager.NameNotFoundException ignored) {
        }
        return installed;
    }

    public static boolean openApplication(@NonNull Context context, @NonNull String applicationId) {
        PackageManager packetManager = context.getPackageManager();
        Intent launchIntent = packetManager.getLaunchIntentForPackage(applicationId);
        if (launchIntent != null) {
            // Null pointer check in case package name was not found.
            context.startActivity(launchIntent);
            return true;
        }
        return false;
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean isTablet() {
        // Verifies if the Generalized Size of the device is XLARGE to be
        // considered a Tablet
        int screenLayout = Resources.getSystem().getConfiguration().screenLayout;
        boolean xlarge = ((screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) >=
                          Configuration.SCREENLAYOUT_SIZE_LARGE);

        // If XLarge, checks if the Generalized Density is at least MDPI
        // (160dpi)
        if (xlarge) {
            int densityDpi = Metrics.getDensityDpi();

            // MDPI=160, DEFAULT=160, DENSITY_HIGH=240, DENSITY_MEDIUM=160,
            // DENSITY_TV=213, DENSITY_XHIGH=320
            if (densityDpi == DisplayMetrics.DENSITY_DEFAULT ||
                densityDpi == DisplayMetrics.DENSITY_HIGH ||
                densityDpi == DisplayMetrics.DENSITY_MEDIUM ||
                densityDpi == DisplayMetrics.DENSITY_TV ||
                densityDpi == DisplayMetrics.DENSITY_XHIGH) {
                // Yes, this is a tablet!
                return true;
            }
        }

        // No, this is not a tablet!
        return false;
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean testConnection(@NonNull Context context) {
        ConnectivityManager connectivityManager =
            (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo info = connectivityManager.getActiveNetworkInfo();
        return info != null && info.isConnectedOrConnecting();
    }
}
