package com.ee.core;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

/**
 * Created by Zinge on 6/5/16.
 */
public class PluginManager {
    private static final Logger _logger = new Logger(PluginManager.class.getName());

    private static boolean _initialized = false;
    private static Context _context = null;
    private static Map<String, PluginProtocol> _plugins = null;

    public static void initialize(Context context, String... pluginNames) {
        if (_initialized) {
            assert (false);
            return;
        }

        _context = context;
        _plugins = new HashMap<>();
        for (String pluginName : pluginNames) {
            loadPlugin(pluginName);
        }

        _initialized = true;
    }

    private static void loadPlugin(@NonNull String pluginName) {
        _logger.debug("loadPlugin: attempt to load plugin: %s", pluginName);

        try {
            Class<?> clazz = Class.forName("com.ee.plugin." + pluginName);
            Constructor<?> constructor = clazz.getConstructor(Context.class);

            Object object = constructor.newInstance(_context);
            assert (object instanceof PluginProtocol);

            _plugins.put(pluginName, (PluginProtocol) object);
        } catch (ClassNotFoundException e) {
            assert (false);
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            assert (false);
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            assert (false);
            e.printStackTrace();
        } catch (InstantiationException e) {
            assert (false);
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            assert (false);
            e.printStackTrace();
        }
    }

    @Nullable
    public static PluginProtocol getProtocolByName(@NonNull String pluginName) {
        assert (_plugins.containsKey(pluginName));
        return _plugins.get(pluginName);
    }

    public static void setDebuggable(boolean debuggable) {
        _logger.setDebuggable(debuggable);
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().setDebuggable(debuggable);
        }
    }

    public static void onStart() {
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onStart();
        }
    }

    public static void onStop() {
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onStop();
        }
    }

    public static void onPause() {
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onPause();
        }
    }

    public static void onResume() {
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onResume();
        }
    }

    public static void onDestroy() {
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            entry.getValue().onDestroy();
        }
    }

    public static boolean onBackPressed() {
        boolean result = false;
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            result = result || entry.getValue().onBackPressed();
        }
        return result;
    }

    public static boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        boolean result = false;
        for (Entry<String, PluginProtocol> entry : _plugins.entrySet()) {
            result = result || entry.getValue().onActivityResult(requestCode, responseCode, data);
        }
        return result;
    }

    @NonNull
    private static String callNative(@NonNull String pluginName,
                                     @NonNull String methodName,
                                     @Nullable String json,
                                     int callbackId_) throws InvocationTargetException {
        Integer callbackId = (callbackId_ == -1 ? null : callbackId_);
        return _callNative(pluginName, methodName, json, callbackId);
    }

    @NonNull
    private static String _callNative(@NonNull String pluginName,
                                      @NonNull String methodName,
                                      @Nullable String json,
                                      @Nullable Integer callbackId) throws
        InvocationTargetException {
        _logger.debug("callNative: pluginName = %s methodName = %s json = %s callbackId = %d",
            pluginName,
            methodName,
            json,
            callbackId);

        PluginProtocol protocol = PluginManager.getProtocolByName(pluginName);
        assert (protocol != null);

        List<Class<?>> classes = new ArrayList<>();
        List<Object> params = new ArrayList<>();
        if (json != null) {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(json);
            assert (dict != null);

            _logger.debug("callNative: dict = " + dict);

            params.add(dict);
            classes.add(Map.class);
        }
        if (callbackId != null) {
            params.add(callbackId);
            classes.add(Integer.class);
        }

        Method method = null;
        try {
            assert (params.size() <= 2);
            if (params.size() == 0) {
                method = protocol.getClass().getMethod(methodName);
            } else if (params.size() == 1) {
                method = protocol.getClass().getMethod(methodName, classes.get(0));
            } else if (params.size() == 2) {
                method = protocol.getClass().getMethod(methodName, classes.get(0), classes.get(1));
            }
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

        assert (method != null);

        Object result = null;
        try {
            result = method.invoke(protocol, params.toArray());
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }

        if (result != null) {
            assert (result instanceof String);
            _logger.debug("callNative: result = %s", result);
            return (String) result;
        }

        String emptyResult = JsonUtils.convertDictionaryToString(new HashMap<String, Object>());
        assert (emptyResult != null);
        _logger.debug("callNative: empty result = %s", emptyResult);
        return emptyResult;
    }
}
