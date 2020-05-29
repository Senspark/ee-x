package com.ee.core;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.core.internal.Utils;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 6/5/16.
 */

public class PluginManager {
    public interface PluginCreator {
        PluginProtocol create(@NonNull Context context, @NonNull IMessageBridge bridge);
    }

    private static final Logger _logger = new Logger(PluginManager.class.getName());

    private Context _context;
    private Activity _activity;
    private final IMessageBridge _bridge;
    private final Map<String, PluginProtocol> _plugins;
    private final Map<String, String> _classes;

    private static class Holder {
        private static final PluginManager Instance = new PluginManager();
    }

    public static PluginManager getInstance() {
        return Holder.Instance;
    }

    private PluginManager() {
        _bridge = MessageBridge.getInstance();
        _plugins = new HashMap<>();
        _classes = new HashMap<>();
        _classes.put("AdMob", "com.ee.admob.AdMob");
        _classes.put("AppLovin", "com.ee.applovin.AppLovin");
        _classes.put("AppsFlyer", "com.ee.appsflyer.AppsFlyer");
        _classes.put("CampaignReceiver", "com.ee.campaignreceiver.CampaignReceiver");
        _classes.put("Facebook", "com.ee.facebook.Facebook");
        _classes.put("FacebookAds", "com.ee.facebook.FacebookAds");
        _classes.put("FirebaseCore", "com.ee.firebase.core.FirebaseCore");
        _classes.put("FirebaseCrashlytics", "com.ee.firebase.crashlytics.FirebaseCrashlytics");
        _classes.put("GoogleAnalytics", "com.ee.google.analytics.GoogleAnalytics");
        _classes.put("IronSource", "com.ee.ironsource.IronSource");
        _classes.put("Notification", "com.ee.notification.Notification");
        _classes.put("Play", "com.ee.play.Play");
        _classes.put("Recorder", "com.ee.recorder.Recorder");
        _classes.put("Store", "com.ee.store.Store");
        _classes.put("Tenjin", "com.ee.tenjin.Tenjin");
        _classes.put("UnityAds", "com.ee.unityads.UnityAds");
        _classes.put("Vungle", "com.ee.vungle.Vungle");
    }

    public void initializePlugins(Context context) {
        _context = context;
        Utils.registerHandlers(_bridge);
    }

    public Context getContext() {
        return _context;
    }

    public Activity getActivity() {
        return _activity;
    }

    public void addPlugin(@NonNull String name) {
        _logger.info("addPlugin: " + name);
        if (_plugins.containsKey(name)) {
            _logger.error("addPlugin: " + name + " already exists!");
            return;
        }
        if (!_classes.containsKey(name)) {
            _logger.error("addPlugin: " + name + " classes doesn't exist!");
            return;
        }
        String className = _classes.get(name);
        try {
            Class<?> clazz = Class.forName(className);
            Constructor<?> constructor = clazz.getConstructor(Context.class, IMessageBridge.class);
            Object object = constructor.newInstance(_context, _bridge);
            _plugins.put(name, (PluginProtocol) object);
        } catch (ClassNotFoundException ex) {
            ex.printStackTrace();
        } catch (NoSuchMethodException ex) {
            ex.printStackTrace();
        } catch (IllegalAccessException ex) {
            ex.printStackTrace();
        } catch (InstantiationException ex) {
            ex.printStackTrace();
        } catch (InvocationTargetException ex) {
            ex.printStackTrace();
        }
    }

    @SuppressWarnings("unused")
    public void removePlugin(@NonNull String pluginName) {
        _logger.info("removePlugin: " + pluginName);
        if (!_plugins.containsKey(pluginName)) {
            _logger.error("removePlugin: " + pluginName + " doesn't exist!");
            return;
        }
        _plugins.remove(pluginName);
    }

    @Nullable
    public PluginProtocol getPlugin(@NonNull String pluginName) {
        if (!_plugins.containsKey(pluginName)) {
            return null;
        }
        return _plugins.get(pluginName);
    }

    public void onCreate(Activity activity) {
        _activity = activity;
        for (String key : _plugins.keySet()) {
            _plugins.get(key).onCreate(activity);
        }
    }

    public void onStart() {
        for (String key : _plugins.keySet()) {
            _plugins.get(key).onStart();
        }
    }

    public void onStop() {
        for (String key : _plugins.keySet()) {
            _plugins.get(key).onStop();
        }
    }

    public void onResume() {
        for (String key : _plugins.keySet()) {
            _plugins.get(key).onResume();
        }
    }

    public void onPause() {
        for (String key : _plugins.keySet()) {
            _plugins.get(key).onPause();
        }
    }

    public void onDestroy() {
        _activity = null;
        for (String key : _plugins.keySet()) {
            _plugins.get(key).onDestroy();
        }
    }

    public void destroy() {
        Utils.deregisterHandlers(_bridge);
        for (String key : _plugins.keySet()) {
            _plugins.get(key).destroy();
        }
        _context = null;
    }

    public boolean onBackPressed() {
        boolean result = false;
        for (String key : _plugins.keySet()) {
            result = result || _plugins.get(key).onBackPressed();
        }
        return result;
    }

    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        boolean result = false;
        for (String key : _plugins.keySet()) {
            result = result || _plugins.get(key).onActivityResult(requestCode, responseCode, data);
        }
        return result;
    }
}