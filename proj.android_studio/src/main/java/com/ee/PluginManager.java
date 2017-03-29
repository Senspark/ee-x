package com.ee.core;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.Map;

/**
 * Created by Zinge on 6/5/16.
 */

public class PluginManager {
    private static final Logger _logger = new Logger(PluginManager.class.getName());

    private Map<String, Object> _plugins;

    private static class Holder {
        private static final PluginManager Instance = new PluginManager();
    }

    public static PluginManager getInstance() {
        return Holder.Instance;
    }

    public void addPlugin(@NonNull String pluginName) {
        _logger.info("addPlugin: " + pluginName);
        if (_plugins.containsKey(pluginName)) {
            _logger.error("addPlugin: " + pluginName + " already exists!");
            return;
        }

        try {
            Class<?> clazz = Class.forName(pluginName);
            Constructor<?> constructor = clazz.getConstructor(Context.class);

            Object object = constructor.newInstance(_context);

            _plugins.put(pluginName, object);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
    }

    public void addPlugin(@NonNull PluginProtocol plugin) {
        _logger.info("addPlugin: " + plugin);
        _plugins.put()
    }

    public void removePlugin(@NonNull String pluginName) {
        _logger.info("removePlugin: " + pluginName);
        if (!_plugins.containsKey(pluginName)) {
            _logger.error("removePlugin: " + pluginName + " doesn't exist!");
            return;
        }
        _plugins.remove(pluginName);
    }

    public static void onStart() {
        /*
        FIXME
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onStart();
        }
        */
    }

    public static void onStop() {
        /*
        FIXME
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onStop();
        }
        */
    }

    public static void onPause() {
        /*
        FIXME
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onPause();
        }
        */
    }

    public static void onResume() {
        /*
        FIXME
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onResume();
        }*/
    }

    public static void onDestroy() {
        /*
        FIXME
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onDestroy();
        }*/
    }

    public static boolean onBackPressed() {
        /*
        FIXME
        boolean result = false;
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            result = result || entry.getValue().onBackPressed();
        }
        return result;
        */
        return false;
    }

    public static boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        /*
        FIXME
        boolean result = false;
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            result = result || entry.getValue().onActivityResult(requestCode, responseCode, data);
        }
        return result;
        */
        return false;
    }
}