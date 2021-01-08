package com.ee.internal;

import com.ee.IMessageBridge;
import com.ee.PluginManager;

class UnityMessageBridge {
    private interface Callback {
        void callCpp(String tag, String message);
    }
    
    private static Callback _callback;
    
    private static void initialize(Callback callback) {
        _callback = callback;
    }
    
    private static String call(String tag, String message) {
        IMessageBridge bridge = PluginManager.getInstance().getBridge();
        return bridge.call(tag, message);
    }
    
    public static void callCpp(String tag, String message) {
        _callback.callCpp(tag, message);
    }
}