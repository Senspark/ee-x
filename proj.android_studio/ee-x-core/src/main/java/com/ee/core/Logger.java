package com.ee.core;

import android.util.Log;

/**
 * Created by enrevol on 4/13/16.
 */
public class Logger {
    private final String _tag;
    private int _logLevel;

    public Logger(String tag) {
        _tag = tag;
        setDebuggable(true);
    }

    public Logger(String tag, boolean debuggable) {
        _tag = tag;
        setDebuggable(debuggable);
    }

    public void setDebuggable(boolean debuggable) {
        if (debuggable) {
            _logLevel = Log.VERBOSE;
        } else {
            _logLevel = Log.INFO;
        }
    }

    public Logger(String tag, int logLevel) {
        _tag = tag;
        _logLevel = logLevel;
    }

    public void error(String msg) {
        if (_logLevel <= Log.ERROR) {
            Log.e(_tag, msg);
        }
    }

    public void error(String msg, Throwable th) {
        if (_logLevel <= Log.ERROR) {
            Log.e(_tag, msg, th);
        }
    }

    public void warn(String msg) {
        if (_logLevel <= Log.WARN) {
            Log.w(_tag, msg);
        }
    }

    public void debug(String msg) {
        if (_logLevel <= Log.DEBUG) {
            Log.d(_tag, msg);
        }
    }

    public void debug(String format, Object... objects) {
        if (_logLevel <= Log.DEBUG) {
            Log.d(_tag, String.format(format, objects));
        }
    }

    public void info(String msg) {
        if (_logLevel <= Log.INFO) {
            Log.i(_tag, msg);
        }
    }

    public void info(String format, Object... objects) {
        if (_logLevel <= Log.INFO) {
            Log.i(_tag, String.format(format, objects));
        }
    }
}
