package com.ee.crashlytics;

import android.util.Log;

import io.fabric.sdk.android.Logger;

/**
 * Created by Zinge on 7/7/16.
 */
public class CrashlyticsLogger implements Logger {
    private int _logLevel;

    public CrashlyticsLogger(int logLevel) {
        _logLevel = logLevel;
    }

    public CrashlyticsLogger() {
        _logLevel = Log.INFO;
    }

    public boolean isLoggable(String tag, int level) {
        return _logLevel <= level;
    }

    public int getLogLevel() {
        return _logLevel;
    }

    public void setLogLevel(int logLevel) {
        _logLevel = logLevel;
    }

    public void d(String tag, String text, Throwable throwable) {
        if (isLoggable(tag, Log.DEBUG)) {
            Log.d(tag, text, throwable);
        }
    }

    public void v(String tag, String text, Throwable throwable) {
        if (isLoggable(tag, Log.VERBOSE)) {
            Log.v(tag, text, throwable);
        }
    }

    public void i(String tag, String text, Throwable throwable) {
        if (isLoggable(tag, Log.INFO)) {
            Log.i(tag, text, throwable);
        }
    }

    public void w(String tag, String text, Throwable throwable) {
        if (isLoggable(tag, Log.WARN)) {
            Log.w(tag, text, throwable);
        }
    }

    public void e(String tag, String text, Throwable throwable) {
        if (isLoggable(tag, Log.ERROR)) {
            Log.e(tag, text, throwable);
        }
    }

    public void d(String tag, String text) {
        d(tag, text, null);
    }

    public void v(String tag, String text) {
        v(tag, text, null);
    }

    public void i(String tag, String text) {
        i(tag, text, null);
    }

    public void w(String tag, String text) {
        w(tag, text, null);
    }

    public void e(String tag, String text) {
        e(tag, text, null);
    }

    public void log(int priority, String tag, String msg) {
        log(priority, tag, msg, false);
    }

    public void log(int priority, String tag, String msg, boolean forceLog) {
        if (isLoggable(tag, priority)) {
            Log.println(priority, tag, msg);
        }
    }
}
