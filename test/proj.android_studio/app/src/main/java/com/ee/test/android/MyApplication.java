package com.ee.test.android;

import android.app.Application;
import android.content.Context;
import android.support.multidex.MultiDex;

import com.ee.admob.AdMob;
import com.ee.applovin.AppLovin;
import com.ee.core.PluginManager;
import com.ee.crashlytics.Crashlytics;
import com.ee.facebook.FacebookAds;
import com.ee.ironsource.IronSource;
import com.ee.notification.Notification;
import com.ee.unityads.UnityAds;
import com.ee.vungle.Vungle;

/**
 * Created by Zinge on 5/17/17.
 */

public class MyApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();

        PluginManager manager = PluginManager.getInstance();
        manager.initializePlugins(this);
        manager.addPlugin(new Crashlytics(this));
        manager.addPlugin(new Notification(this));
        manager.addPlugin(new AdMob(this));
        manager.addPlugin(new AppLovin(this));
        manager.addPlugin(new FacebookAds(this));
        manager.addPlugin(new IronSource());
        manager.addPlugin(new UnityAds());
        manager.addPlugin(new Vungle(this));
    }

    @Override
    protected void attachBaseContext(Context context) {
        super.attachBaseContext(context);
        MultiDex.install(this);
    }
}
