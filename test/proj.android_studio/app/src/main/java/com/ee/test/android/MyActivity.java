package com.ee.test.android;

import android.content.Intent;
import android.os.Bundle;

import com.ee.core.Logger;
import com.ee.core.Metrics;
import com.ee.core.PluginManager;
import com.ee.crashlytics.Crashlytics;
import com.ee.facebook.FacebookAds;
import com.ee.ironsource.IronSource;
import com.ee.notification.Notification;
import com.ee.unityads.UnityAds;
import com.ee.vungle.Vungle;

import org.cocos2dx.lib.Cocos2dxActivity;

/**
 * Created by Zinge on 5/17/17.
 */

public class MyActivity extends Cocos2dxActivity {
    private static final Logger _logger = new Logger(MyActivity.class.getName());

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        _logger.debug("onCreate: begin.");
        super.onCreate(savedInstanceState);
        Metrics.initialize();
        PluginManager manager = PluginManager.getInstance();
        manager.addPlugin(new Crashlytics(this));
        manager.addPlugin(new Notification(this));
        manager.addPlugin(new FacebookAds(this));
        manager.addPlugin(new UnityAds(this));
        manager.addPlugin(new IronSource(this));
        manager.addPlugin(new Vungle(this));

        _logger.debug("onCreate: end.");
    }

    @Override
    protected void onStart() {
        _logger.debug("onStart: begin.");
        super.onStart();
        PluginManager.getInstance().onStart();
        _logger.debug("onStart: end.");
    }

    @Override
    protected void onStop() {
        _logger.debug("onStop: begin.");
        super.onStop();
        PluginManager.getInstance().onStop();
        _logger.debug("onStop: end.");
    }

    @Override
    protected void onResume() {
        _logger.debug("onResume: begin.");
        super.onResume();
        PluginManager.getInstance().onResume();
        _logger.debug("onResume: end.");
    }

    @Override
    protected void onPause() {
        _logger.debug("onPause: begin.");
        super.onPause();
        PluginManager.getInstance().onPause();
        _logger.debug("onPause: end.");
    }

    @Override
    protected void onDestroy() {
        _logger.debug("onDestroy: begin.");
        super.onDestroy();
        PluginManager.getInstance().onDestroy();
        _logger.debug("onDestroy: end.");
    }

    @Override
    public void onBackPressed() {
        if (PluginManager.getInstance().onBackPressed()) {
            return;
        }
        super.onBackPressed();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        _logger.debug(
            "onActivityResult: requestCode = " + requestCode + " resultCode = " + resultCode +
            " data = " + data);
        if (PluginManager.getInstance().onActivityResult(requestCode, resultCode, data)) {
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }
}
