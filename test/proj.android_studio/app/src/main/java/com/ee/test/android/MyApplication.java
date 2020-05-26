package com.ee.test.android;

import android.app.Application;
import android.content.Context;

import androidx.multidex.MultiDex;

import com.ee.admob.AdMob;
import com.ee.applovin.AppLovin;
import com.ee.appsflyer.AppsFlyer;
import com.ee.campaignreceiver.CampaignReceiver;
import com.ee.core.PluginManager;
import com.ee.facebook.Facebook;
import com.ee.facebook.FacebookAds;
import com.ee.firebase.core.FirebaseCore;
import com.ee.firebase.crashlytics.FirebaseCrashlytics;
import com.ee.firebase.performance.FirebasePerformance;
import com.ee.google.analytics.GoogleAnalytics;
import com.ee.ironsource.IronSource;
import com.ee.notification.Notification;
import com.ee.play.Play;
import com.ee.recorder.Recorder;
import com.ee.tenjin.Tenjin;
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
        manager.addPlugin(new AdMob(this));
        manager.addPlugin(new AppLovin(this));
        manager.addPlugin(new AppsFlyer(this));
        manager.addPlugin(new CampaignReceiver(this));
        manager.addPlugin(new Facebook());
        manager.addPlugin(new FacebookAds(this));
        manager.addPlugin(new FirebaseCore(this));
        manager.addPlugin(new FirebaseCrashlytics());
        manager.addPlugin(new FirebasePerformance());
        manager.addPlugin(new GoogleAnalytics(this));
        manager.addPlugin(new IronSource());
        manager.addPlugin(new Notification(this));
        manager.addPlugin(new Play());
        manager.addPlugin(new Recorder(this));
        manager.addPlugin(new Tenjin());
        manager.addPlugin(new UnityAds());
        manager.addPlugin(new Vungle(this));
    }

    @Override
    protected void attachBaseContext(Context context) {
        super.attachBaseContext(context);
        MultiDex.install(this);
    }
}
