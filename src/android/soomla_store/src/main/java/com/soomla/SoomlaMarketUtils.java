package com.soomla;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

/**
 * Created by oriargov on 7/28/14.
 */
public class SoomlaMarketUtils {
    private static final String AMAZON_INSTALLER = "com.amazon.venezia";
    private static final String ANDROID_INSTALLER = "com.android.vending";

    public static void openMarketAppPage(final Context context) {
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        final String marketAppPageUrl = getMarketAppPageUrl(context);
        if (marketAppPageUrl == null) {
            SoomlaUtils.LogError(TAG, "no valid market app page url found!");
            return;
        }

        intent.setData(Uri.parse(marketAppPageUrl));
        context.startActivity(intent);
    }

    public static String getMarketAppPageUrl(Context context) {
        String url = null;
        final String packageName = context.getPackageName();
        if (isInstalledFromGooglePlay(context)) {
            url = String.format("market://details?id=%s", packageName);
        }
        else if (isInstalledFromAmazonStore(context)) {
            url = String.format("amzn://apps/android?p=%s", packageName);
//          url = String.format("http://www.amazon.com/gp/mas/dl/android?p=%s", packageName);
        }

        return url;
    }

//    public static String getMarketDeveloperPageUrl(Context context) {
//        if (isInstalledFromGooglePlay(context)) {
//            return "market://search?q=pub:<publisher_name>";
//        }
//        else if (isInstalledFromAmazonStore(context)) {
//            return "amzn://apps/android?p=%s&showAll=1";
////            return "http://www.amazon.com/gp/mas/dl/android?p=%s";
//        }
//        else {
//            return null;
//        }
//    }

    public static boolean isInstalledFromGooglePlay(Context context) {
        return isPackageInstaller(context, ANDROID_INSTALLER);
    }

    public static boolean isInstalledFromAmazonStore(Context context) {
        return isPackageInstaller(context, AMAZON_INSTALLER);

//        boolean sandboxMode = false;
//        boolean amazonIsInstaller = isPackageInstaller(context, AMAZON_INSTALLER);
//        sandboxMode = !amazonIsInstaller && !hasAmazonClasses();
//        SoomlaUtils.LogDebug(TAG, "isPackageInstaller() sandBox: " + sandboxMode);
//        return !sandboxMode;
    }

    public static boolean isPackageInstaller(Context appContext, String installer) {
        String installerPackageName = appContext.getPackageManager().getInstallerPackageName(appContext.getPackageName());
        return installerPackageName != null && installerPackageName.equals(installer);
    }

    /**
     * Tries to load Amazon <code>com.amazon.android.Kiwi</code> class.
     * <p>
     * Submitted .apk is not published to Amazon as is. It is re-packed with several Amazon-specific
     * classes. We examine such classes to understand whether app is delivered by Amazon
     */
    public static boolean hasAmazonClasses() {
        boolean result;
        synchronized (SoomlaUtils.class) {
            try {
                ClassLoader localClassLoader = SoomlaUtils.class.getClassLoader();
                localClassLoader.loadClass("com.amazon.android.Kiwi");
                result = true;
            } catch (Throwable localThrowable) {
                SoomlaUtils.LogDebug(TAG, "hasAmazonClasses() cannot load class com.amazon.android.Kiwi");
                result = false;
            }
        }
        return result;
    }

    /** Private Members **/

    private static String TAG = "SOOMLA SoomlaStoreDetector"; //used for Log messages
}
