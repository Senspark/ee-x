package com.ee.test.android;

import android.app.Application;
import android.content.Context;

import androidx.multidex.MultiDex;

import com.ee.core.PluginManager;

/**
 * Created by Zinge on 5/17/17.
 */

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        PluginManager manager = PluginManager.getInstance();
        manager.initializePlugins(this);
        manager.addPlugin("AdMob");
        manager.addPlugin("AppLovin");
        manager.addPlugin("AppsFlyer");
        manager.addPlugin("CampaignReceiver");
        manager.addPlugin("Facebook");
        manager.addPlugin("FacebookAds");
        manager.addPlugin("FirebaseCore");
        manager.addPlugin("FirebaseCrashlytics");
        manager.addPlugin("FirebasePerformance");
        manager.addPlugin("GoogleAnalytics");
        manager.addPlugin("IronSource");
        manager.addPlugin("Notification");
        manager.addPlugin("Play");
        manager.addPlugin("Recorder");
        manager.addPlugin("Store");
        manager.addPlugin("Tenjin");
        manager.addPlugin("UnityAds");
        manager.addPlugin("Vungle");
    }

    @Override
    protected void attachBaseContext(Context context) {
        super.attachBaseContext(context);
        MultiDex.install(this);
    }
}
