package com.ee.internal;

import android.app.Activity;

import com.ee.IMessageBridge;
import com.ee.PluginManager;

public class UnityPluginManager {
    public static boolean initializePlugins() {
        IMessageBridge bridge = new MessageBridge(UnityMessageBridge::callCpp);
        return PluginManager.getInstance().initializePlugins(bridge);
    }
    
    public static Activity getActivity() {
        return PluginManager.getInstance().getActivity();
    }
    
    public static void setActivity(Activity activity) {
        PluginManager.getInstance().setActivity(activity);
    }
    
    public static boolean addPlugin(String name) {
        return PluginManager.getInstance().addPlugin(name);
    }
    
    public static boolean removePlugin(String name) {
        return PluginManager.getInstance().removePlugin(name);
    }
}