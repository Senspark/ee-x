package com.ee.core.internal;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.view.DisplayCutout;
import android.view.Surface;
import android.view.View;
import android.view.WindowInsets;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.PluginManager;
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.instantapps.InstantApps;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.security.MessageDigest;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.TimeUnit;

import io.reactivex.rxjava3.core.Single;
import io.reactivex.rxjava3.schedulers.Schedulers;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/9/17.
 */
public class Utils {
    private static final int INSTALL_APP_REQUEST_CODE = 0x10;

    private static final Logger _logger = new Logger(Utils.class.getName());

    private static final String k__isMainThread = "Utils_isMainThread";
    private static final String k__runOnUiThread = "Utils_runOnUiThread";
    private static final String k__runOnUiThreadDelayed = "Utils_runOnUiThreadDelayed";
    private static final String k__runOnUiThreadCallback = "Utils_runOnUiThreadCallback";

    private static final String k__isApplicationInstalled = "Utils_isApplicationInstalled";
    private static final String k__openApplication = "Utils_openApplication";

    private static final String k__getApplicationId = "Utils_getApplicationId";
    private static final String k__getApplicationName = "Utils_getApplicationName";
    private static final String k__getVersionName = "Utils_getVersionName";
    private static final String k__getVersionCode = "Utils_getVersionCode";

    private static final String k__getSHA1CertificateFingerprint = "Utils_getSHA1CertificateFingerprint";
    private static final String k__isInstantApp = "Utils_isInstantApp";
    private static final String k__isTablet = "Utils_isTablet";
    private static final String k__getDensity = "Utils_getDensity";
    private static final String k__getDeviceId = "Utils_getDeviceId";
    private static final String k__getSafeInset = "Utils_getSafeInset";

    private static final String k__sendMail = "Utils_sendMail";
    private static final String k__testConnection = "Utils_testConnection";
    private static final String k__showInstallPrompt = "Utils_showInstallPrompt";

    public static FrameLayout getRootView(Activity activity) {
        return (FrameLayout) activity.findViewById(android.R.id.content).getRootView();
    }

    @NonNull
    public static String toString(boolean value) {
        return value ? "true" : "false";
    }

    @NonNull
    public static Boolean toBoolean(@NonNull String value) {
        assertThat(value).isAnyOf("true", "false");
        return "true".equals(value);
    }

    /// https://stackoverflow.com/questions/1069066/get-current-stack-trace-in-java
    @NonNull
    public static StackTraceElement[] getCurrentStackTrace() {
        return Thread.currentThread().getStackTrace();
    }

    public static void checkMainThread() {
        if (!isMainThread()) {
            _logger.error("Current thread is not the main thread");
            for (StackTraceElement e : getCurrentStackTrace()) {
                _logger.warn(e.toString());
            }
            assertThat(false).isTrue();
        }
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean isMainThread() {
        return Thread.currentThread() == Looper.getMainLooper().getThread();
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean runOnUiThread(Runnable callback) {
        if (isMainThread()) {
            callback.run();
            return true;
        }
        Handler handler = new Handler(Looper.getMainLooper());
        boolean result = handler.post(callback);
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable");
        }
        return false;
    }

    @SuppressWarnings("WeakerAccess")
    public static void runOnUiThreadDelayed(float seconds, Runnable callback) {
        Handler handler = new Handler(Looper.getMainLooper());
        boolean result = handler.postDelayed(callback, (long) seconds * 1000);
        if (!result) {
            _logger.error("runOnUiThread: failed to post the runnable");
        }
    }

    public static void registerHandlers(IMessageBridge bridge) {
        bridge.registerHandler(k__isMainThread, message -> toString(isMainThread()));

        bridge.registerHandler(k__runOnUiThread, message ->
            toString(runOnUiThread(() -> bridge.callCpp(k__runOnUiThreadCallback))));

        bridge.registerAsyncHandler(k__runOnUiThreadDelayed, (message, resolver) -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            Double delay = (Double) dict.get("delay");
            assertThat(delay).isNotNull();

            runOnUiThreadDelayed(delay.floatValue(), () -> resolver.resolve(""));
        });

        bridge.registerHandler(k__isApplicationInstalled, message -> {
            Context context = PluginManager.getInstance().getContext();
            @SuppressWarnings("UnnecessaryLocalVariable")
            String applicationId = message;
            return toString(isApplicationInstalled(context, applicationId));
        });

        bridge.registerHandler(k__openApplication, message -> {
            Context context = PluginManager.getInstance().getContext();
            @SuppressWarnings("UnnecessaryLocalVariable")
            String applicationId = message;
            return toString(openApplication(context, applicationId));
        });

        bridge.registerHandler(k__getApplicationId, message -> {
            Context context = PluginManager.getInstance().getContext();
            return getApplicationId(context);
        });

        bridge.registerHandler(k__getApplicationName, message -> {
            Context context = PluginManager.getInstance().getContext();
            return getApplicationName(context);
        });

        bridge.registerHandler(k__getVersionName, message -> {
            Context context = PluginManager.getInstance().getContext();
            return getVersionName(context);
        });

        bridge.registerHandler(k__getVersionCode, message -> {
            Context context = PluginManager.getInstance().getContext();
            return String.valueOf(getVersionCode(context));
        });

        bridge.registerHandler(k__getSHA1CertificateFingerprint, message -> {
            Context context = PluginManager.getInstance().getContext();
            return getSHA1CertificateFingerprint(context);
        });

        bridge.registerHandler(k__isInstantApp, message -> {
            Context context = PluginManager.getInstance().getContext();
            return toString(isInstantApp(context));
        });

        bridge.registerHandler(k__isTablet, message -> toString(isTablet()));

        bridge.registerHandler(k__getDensity, message -> {
            double density = getDensity();
            return String.valueOf(density);
        });

        bridge.registerAsyncHandler(k__getDeviceId, (message, resolver) -> {
            Context context = PluginManager.getInstance().getContext();
            getDeviceId(context)
                .subscribe(
                    resolver::resolve,
                    exception -> resolver.resolve(""));
        });

        bridge.registerHandler(k__getSafeInset, message -> {
            Activity activity = PluginManager.getInstance().getActivity();
            SafeInset inset = getSafeInset(activity);
            Map<String, Object> dict = new HashMap<>();
            dict.put("left", inset.left);
            dict.put("right", inset.right);
            dict.put("top", inset.top);
            dict.put("bottom", inset.bottom);
            return Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict));
        });

        bridge.registerHandler(k__sendMail, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String recipient = (String) dict.get("recipient");
            String subject = (String) dict.get("subject");
            String body = (String) dict.get("body");
            assertThat(recipient).isNotNull();
            assertThat(subject).isNotNull();
            assertThat(body).isNotNull();

            Context context = PluginManager.getInstance().getContext();
            return toString(sendMail(context, recipient, subject, body));
        });

        bridge.registerAsyncHandler(k__testConnection, (message, resolver) -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String hostName = (String) dict.get("host_name");
            Double timeOut = (Double) dict.get("time_out");
            assertThat(hostName).isNotNull();
            assertThat(timeOut).isNotNull();

            Context context = PluginManager.getInstance().getContext();
            testConnection(context, hostName, timeOut.floatValue())
                .subscribe(
                    result -> resolver.resolve(Utils.toString(result)),
                    exception -> resolver.resolve(Utils.toString(false)));
        });

        bridge.registerHandler(k__showInstallPrompt, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String url = (String) dict.get("url");
            String referrer = (String) dict.get("referrer");
            assertThat(url).isNotNull();
            assertThat(referrer).isNotNull();

            Activity activity = PluginManager.getInstance().getActivity();
            assertThat(activity).isNotNull();

            showInstantPrompt(url, referrer, activity);
            return "";
        });
    }

    public static void deregisterHandlers(IMessageBridge bridge) {
        bridge.deregisterHandler(k__isMainThread);
        bridge.deregisterHandler(k__runOnUiThread);
        bridge.deregisterHandler(k__runOnUiThreadDelayed);
        bridge.deregisterHandler(k__isApplicationInstalled);
        bridge.deregisterHandler(k__openApplication);
        bridge.deregisterHandler(k__getApplicationId);
        bridge.deregisterHandler(k__getVersionName);
        bridge.deregisterHandler(k__getVersionCode);
        bridge.deregisterHandler(k__getSHA1CertificateFingerprint);
        bridge.deregisterHandler(k__isInstantApp);
        bridge.deregisterHandler(k__isTablet);
        bridge.deregisterHandler(k__getDensity);
        bridge.deregisterHandler(k__getDeviceId);
        bridge.deregisterHandler(k__getSafeInset);
        bridge.deregisterHandler(k__sendMail);
        bridge.deregisterHandler(k__testConnection);
        bridge.deregisterHandler(k__showInstallPrompt);
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

    @SuppressWarnings("WeakerAccess")
    public static boolean openApplication(@NonNull Context context, @NonNull String
        applicationId) {
        PackageManager packetManager = context.getPackageManager();
        Intent launchIntent = packetManager.getLaunchIntentForPackage(applicationId);
        if (launchIntent != null) {
            // Null pointer check in case package name was not found.
            context.startActivity(launchIntent);
            return true;
        }
        return false;
    }

    private static String getApplicationId(@NonNull Context context) {
        return context.getPackageName();
    }

    private static String getApplicationName(@NonNull Context context) {
        PackageManager pm = context.getPackageManager();
        try {
            ApplicationInfo ai = pm.getApplicationInfo(context.getPackageName(), 0);
            return (String) pm.getApplicationLabel(ai);
        } catch (PackageManager.NameNotFoundException e) {
            return "";
        }
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
    @NonNull
    public static String getSHA1CertificateFingerprint(@NonNull Context context) {
        return getCertificateFingerprint(context, "SHA1");
    }

    @NonNull
    private static String getCertificateFingerprint(@NonNull Context context, @NonNull String algorithm) {
        String hash = "";
        try {
            PackageManager packetManager = context.getPackageManager();
            int flags = PackageManager.GET_SIGNATURES;
            PackageInfo info = packetManager.getPackageInfo(context.getPackageName(), flags);
            Signature signature = info.signatures[0];
            MessageDigest digest = MessageDigest.getInstance(algorithm);
            hash = toHexString(digest.digest(signature.toByteArray()));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return hash;
    }

    private static final char[] HEX_ARRAY = "0123456789ABCDEF".toCharArray();

    @NonNull
    private static String toHexString(@NonNull byte[] bytes) {
        char[] chars = new char[bytes.length * 2];
        for (int i = 0; i < bytes.length; ++i) {
            int v = bytes[i] & 0xFF;
            chars[i * 2] = HEX_ARRAY[v >> 4];
            chars[i * 2 + 1] = HEX_ARRAY[v & 0x0F];
        }
        return new String(chars);
    }

    private static boolean isInstantApp(@NonNull Context context) {
        return com.google.android.gms.common.wrappers.InstantApps.isInstantApp(context);
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
            int densityDpi = getDensityDpi();

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

    private static Single<String> getDeviceId(@NonNull Context context) {
        return Single
            .fromCallable(() -> {
                AdvertisingIdClient.Info info = AdvertisingIdClient
                    .getAdvertisingIdInfo(context.getApplicationContext());
                if (info.isLimitAdTrackingEnabled()) {
                    return "";
                }
                return info.getId();
            });
    }

    private static class SafeInset {
        public int left;
        public int right;
        public int top;
        public int bottom;
    }

    private static void calculateSafeInset(Activity activity, int notchSize, SafeInset inset) {
        // https://stackoverflow.com/questions/3663665/how-can-i-get-the-current-screen-orientation
        int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
        switch (rotation) {
            case Surface.ROTATION_0:
                inset.top = notchSize;
                break;
            case Surface.ROTATION_90:
                inset.left = notchSize;
                break;
            case Surface.ROTATION_180:
                inset.bottom = notchSize;
                break;
            case Surface.ROTATION_270:
                inset.right = notchSize;
                break;
        }
    }

    private static boolean getSafeInset_Huawei(Activity activity, SafeInset inset)
        throws IllegalAccessException, InvocationTargetException, NoSuchMethodException {
        Class<?> class_HwNotchSizeUtil;
        try {
            ClassLoader classLoader = activity.getClassLoader();
            class_HwNotchSizeUtil = classLoader.loadClass("com.huawei.android.util.HwNotchSizeUtil");
            Method method_hasNotchInScreen = class_HwNotchSizeUtil.getMethod("hasNotchInScreen");
            boolean hasFeature = (boolean) method_hasNotchInScreen.invoke(class_HwNotchSizeUtil);
            if (!hasFeature) {
                return false;
            }
        } catch (Exception e) {
            return false;
        }
        Method method_getNotchSize = class_HwNotchSizeUtil.getMethod("getNotchSize");
        int[] size = (int[]) method_getNotchSize.invoke(class_HwNotchSizeUtil);
        int notchWidth = size[0];
        int notchHeight = size[1];
        calculateSafeInset(activity, notchHeight, inset);
        return true;
    }

    private static boolean getSafeInset_Oppo(Activity activity, SafeInset inset)
        throws
        ClassNotFoundException, IllegalAccessException, InstantiationException, InvocationTargetException, NoSuchMethodException {
        if (!activity.getPackageManager().hasSystemFeature("com.oppo.feature.screen.heteromorphism")) {
            return false;
        }
        @SuppressLint("PrivateApi")
        Class<?> cls = Class.forName("android.os.SystemProperties");
        Method hideMethod = cls.getMethod("get", String.class);
        Object object = cls.newInstance();
        String value = (String) hideMethod.invoke(object, "ro.oppo.screen.heteromorphism");

        // [378,0:702,80]
        String[] texts = value.split("[,:]");
        int[] values = new int[texts.length];
        try {
            for (int i = 0; i < texts.length; ++i) {
                values[i] = Integer.parseInt(texts[i]);
            }
        } catch (NumberFormatException e) {
            values = null;
        }
        if (values != null && values.length == 4) {
            int notchHeight = values[3];
            calculateSafeInset(activity, notchHeight, inset);
        }
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public static SafeInset getSafeInset(@NonNull Activity activity) {
        SafeInset inset = new SafeInset();
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.P) {
            try {
                // https://stackoverflow.com/questions/51743622/how-to-handle-notchdisplay-cutout-in-android-api-lower-than-28
                if (getSafeInset_Oppo(activity, inset)) {
                    return inset;
                }
                if (getSafeInset_Huawei(activity, inset)) {
                    return inset;
                }
            } catch (Exception ex) {
                _logger.error("getSafeInset", ex);
            }
        } else {
            View decorView = activity.getWindow().getDecorView();
            WindowInsets insets = decorView.getRootWindowInsets();
            DisplayCutout cutout = insets.getDisplayCutout();
            if (cutout == null) {
                // Doesn't have cutout.
            } else {
                inset.left = cutout.getSafeInsetLeft();
                inset.right = cutout.getSafeInsetRight();
                inset.top = cutout.getSafeInsetTop();
                inset.bottom = cutout.getSafeInsetBottom();
            }
        }
        return inset;
    }

    @SuppressWarnings("WeakerAccess")
    public static boolean sendMail(@NonNull Context context, @NonNull String
        recipient, @NonNull String subject,
                                   @NonNull String body) {
        Intent intent = new Intent(Intent.ACTION_SEND) //
            .setType("message/rfc822") //
            .putExtra(Intent.EXTRA_EMAIL, new String[]{recipient}) //
            .putExtra(Intent.EXTRA_SUBJECT, subject) //
            .putExtra(Intent.EXTRA_TEXT, body);
        try {
            // Fix Calling startActivity() from outside of an Activity  context requires the
            // FLAG_ACTIVITY_NEW_TASK flag. Is this really what you want?
            Intent chooserIntent = Intent.createChooser(intent, "Send mail...");
            chooserIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(chooserIntent);
            return true;
        } catch (ActivityNotFoundException ex) {
            // There are no email clients installed.
            ex.printStackTrace();
        }
        return false;
    }


    /// This methods only test if internet is connected or connecting.
    @SuppressWarnings("WeakerAccess")
    public static boolean isInternetAvailable(@NonNull Context context) {
        ConnectivityManager connectivityManager =
            (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
        assert connectivityManager != null;
        NetworkInfo info = connectivityManager.getActiveNetworkInfo();
        return info != null && info.isConnectedOrConnecting();
    }

    /// https://stackoverflow.com/questions/9570237/android-check-internet-connection
    /// https://stackoverflow.com/questions/2758612/executorservice-that-interrupts-tasks-after-a-timeout
    public static Single<Boolean> testConnection(@NonNull Context context, @NonNull String
        hostName, float timeOut) {
        return Single
            .<Boolean>create(emitter -> {
                try {
                    boolean isAvailable = isInternetAvailable(context);
                    if (!isAvailable) {
                        emitter.onSuccess(false);
                        return;
                    }
                    InetAddress address = InetAddress.getByName(hostName);
                    emitter.onSuccess(!address.equals(""));
                } catch (UnknownHostException ex) {
                    if (emitter.isDisposed()) {
                        // Time-out.
                    } else {
                        emitter.onError(ex);
                    }
                }
            })
            .subscribeOn(Schedulers.io())
            .timeout((long) (timeOut * 1000), TimeUnit.MILLISECONDS);
    }

    public static void showInstantPrompt(@NonNull String url, @NonNull String
        referrer, @NonNull Activity activity) {
        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url)).addCategory(Intent.CATEGORY_BROWSABLE);
        InstantApps.showInstallPrompt(activity, intent, INSTALL_APP_REQUEST_CODE, referrer);
    }
}
