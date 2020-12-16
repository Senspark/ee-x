package com.ee.internal;

import com.ee.Thread;

class UnityThread {
    private interface Callback {
        void runOnMainThreadCallback();
        void runOnMainThreadDelayedCallback(int key);
    }

    private static Callback _callback;

    private static void initialize(Callback callback) {
        _callback = callback;
    }
    
    private static boolean isMainThread() {
        return Thread.isMainThread();
    }
    
    private static boolean runOnMainThread() {
        return Thread.runOnMainThread(() -> _callback.runOnMainThreadCallback());
    }
    
    private static void ee_runOnMainThreadDelayed(int key, float delay) {
        Thread.runOnMainThreadDelayed(delay, () -> _callback.runOnMainThreadDelayedCallback(key));
    }
}