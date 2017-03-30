package com.ee;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 6/5/16.
 */

public class PluginManager {
    private static final Logger _logger = new Logger(PluginManager.class.getName());

    private Map<String, PluginProtocol> _plugins;

    private static class Holder {
        private static final PluginManager Instance = new PluginManager();
    }

    public static PluginManager getInstance() {
        return Holder.Instance;
    }

    private PluginManager() {
        _plugins = new HashMap<>();
    }

    @SuppressWarnings({"unused", "TryWithIdenticalCatches"})
    public void addPlugin(@NonNull String pluginName) {
        _logger.info("addPlugin: " + pluginName);
        if (_plugins.containsKey(pluginName)) {
            _logger.error("addPlugin: " + pluginName + " already exists!");
            return;
        }

        try {
            Class<?> clazz = Class.forName(pluginName);
            Constructor<?> constructor = clazz.getConstructor(Context.class);

            Object object = constructor.newInstance();

            _plugins.put(pluginName, (PluginProtocol) object);
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
        if (_plugins.containsKey(plugin.getPluginName())) {
            _logger.error("addPlugin: " + plugin.getPluginName() + " already exists!");
            return;
        }
        _plugins.put(plugin.getPluginName(), plugin);
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

    public void onStart() {
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onStart();
        }
    }

    public void onStop() {
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onStop();
        }
    }

    public void onPause() {
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onPause();
        }
    }

    public void onResume() {
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onResume();
        }
    }

    public void onDestroy() {
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onDestroy();
        }
    }

    public boolean onBackPressed() {
        boolean result = false;
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            result = result || entry.getValue().onBackPressed();
        }
        return result;
    }

    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        boolean result = false;
        for (Map.Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            result = result || entry.getValue().onActivityResult(requestCode, responseCode, data);
        }
        return result;
    }
}