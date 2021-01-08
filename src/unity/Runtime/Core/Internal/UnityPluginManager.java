package com.ee.internal;

import android.app.Activity;

import com.ee.IMessageBridge;
import com.ee.PluginManager;

import kotlin.Unit;

public class UnityPluginManager {
    public static void initializePlugins(String version) {
        PluginManager.getInstance().initializePlugins(version, (tag, message) -> {
            UnityMessageBridge.callCpp(tag, message);          
            return Unit.INSTANCE;
        });
    }
    
    public static Activity getActivity() {
        return PluginManager.getInstance().getActivity();
    }
    
    public static void setActivity(Activity activity) {
        PluginManager.getInstance().setActivity(activity);
    }
    
    public static void addPlugin(String name) {
        PluginManager.getInstance().addPlugin(name);
    }
    
    public static void removePlugin(String name) {
        PluginManager.getInstance().removePlugin(name);
    }
}