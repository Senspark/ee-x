package com.ee.core;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.core.internal.Utils;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 6/5/16.
 */

public class PluginManager {
    private interface PluginExecutor {
        void execute(@NonNull IPlugin plugin);
    }

    private static final Logger _logger = new Logger(PluginManager.class.getName());

    private Context _context;
    private Activity _activity;
    private String _activityClassName;
    private final IMessageBridge _bridge;
    private final Map<String, IPlugin> _plugins;
    private final Map<String, String> _classes;
    private final Application.ActivityLifecycleCallbacks _lifecycleCallbacks;

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
        _classes.put("FirebasePerformance", "com.ee.firebase.performance.FirebasePerformance");
        _classes.put("GoogleAnalytics", "com.ee.google.analytics.GoogleAnalytics");
        _classes.put("IronSource", "com.ee.ironsource.IronSource");
        _classes.put("Notification", "com.ee.notification.Notification");
        _classes.put("Play", "com.ee.play.Play");
        _classes.put("Recorder", "com.ee.recorder.Recorder");
        _classes.put("Store", "com.ee.store.Store");
        _classes.put("Tenjin", "com.ee.tenjin.Tenjin");
        _classes.put("UnityAds", "com.ee.unityads.UnityAds");
        _classes.put("Vungle", "com.ee.vungle.Vungle");

        _lifecycleCallbacks = new Application.ActivityLifecycleCallbacks() {
            @Override
            public void onActivityCreated(@NonNull Activity activity,
                                          @Nullable Bundle savedInstanceState) {
                if (activity.getClass().getName().equals(_activityClassName)) {
                    if (_activity == null) {
                        if (activity.isTaskRoot()) {
                            _activity = activity;
                            executePlugins(plugin -> plugin.onCreate(activity));
                        } else {
                            _logger.warn("onActivityCreated: is not root activity");
                        }
                    } else {
                        _logger.warn("onActivityCreated: invalid activity");
                    }
                }
            }

            @Override
            public void onActivityStarted(@NonNull Activity activity) {
                if (activity.getClass().getName().equals(_activityClassName)) {
                    if (_activity == activity) {
                        executePlugins(IPlugin::onStart);
                    } else {
                        _logger.warn("onActivityStarted: invalid activity");
                    }
                }
            }

            @Override
            public void onActivityResumed(@NonNull Activity activity) {
                if (activity.getClass().getName().equals(_activityClassName)) {
                    if (_activity == activity) {
                        executePlugins(IPlugin::onResume);
                    } else {
                        _logger.warn("onActivityResumed: invalid activity");
                    }
                }
            }

            @Override
            public void onActivityPaused(@NonNull Activity activity) {
                if (activity.getClass().getName() == _activityClassName) {
                    if (_activity == activity) {
                        executePlugins(IPlugin::onPause);
                    } else {
                        _logger.warn("onActivityPaused: invalid activity");
                    }
                }
            }

            @Override
            public void onActivityStopped(@NonNull Activity activity) {
                if (activity.getClass().getName().equals(_activityClassName)) {
                    if (_activity == activity) {
                        executePlugins(IPlugin::onStop);
                    } else {
                        _logger.warn("onActivityStopped: invalid activity");
                    }
                }
            }

            @Override
            public void onActivitySaveInstanceState(@NonNull Activity activity,
                                                    @NonNull Bundle outState) {
            }

            @Override
            public void onActivityDestroyed(@NonNull Activity activity) {
                if (activity.getClass().getName().equals(_activityClassName)) {
                    if (_activity == activity) {
                        _activity = null;
                        executePlugins(IPlugin::onDestroy);
                    } else {
                        _logger.warn("onActivityDestroyed: invalid activity");
                    }
                }
            }
        };
    }

    public void initializePlugins(Context context) {
        _context = context;

        // Register lifecycle callbacks.
        if (context instanceof Application) {
            ((Application) context).registerActivityLifecycleCallbacks(_lifecycleCallbacks);
        } else {
            assertThat(true).isFalse();
        }

        // Find main activity.
        String packageName = context.getPackageName();
        Intent launchIntent = context.getPackageManager().getLaunchIntentForPackage(packageName);
        _activityClassName = launchIntent.getComponent().getClassName();
        assertThat(_activityClassName).isNotNull();

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
            _plugins.put(name, (IPlugin) object);
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

    public void removePlugin(@NonNull String pluginName) {
        _logger.info("removePlugin: " + pluginName);
        if (!_plugins.containsKey(pluginName)) {
            _logger.error("removePlugin: " + pluginName + " doesn't exist!");
            return;
        }
        _plugins.remove(pluginName);
    }

    @Nullable
    public IPlugin getPlugin(@NonNull String pluginName) {
        if (!_plugins.containsKey(pluginName)) {
            return null;
        }
        return _plugins.get(pluginName);
    }

    public void destroy() {
        Utils.deregisterHandlers(_bridge);
        for (String key : _plugins.keySet()) {
            _plugins.get(key).destroy();
        }
        _context = null;
    }

    /// FIXME: to be removed (used in Recorder plugin).
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        boolean result = false;
        for (String key : _plugins.keySet()) {
            result = result || _plugins.get(key).onActivityResult(requestCode, responseCode, data);
        }
        return result;
    }

    private void executePlugins(PluginExecutor executor) {
        for (String key : _plugins.keySet()) {
            IPlugin plugin = _plugins.get(key);
            executor.execute(plugin);
        }
    }
}