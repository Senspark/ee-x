package com.ee.core;

import android.util.Log;

/**
 * Created by enrevol on 4/13/16.
 */
public class Logger {
    private String tag;

    public Logger(String tag) {
        super();
        this.tag = tag;
    }

    public void error(String msg) {
        Log.e(tag, msg);
    }

    public void error(String msg, Throwable th) {
        Log.e(tag, msg, th);
    }

    public void debug(String msg) {
        Log.d(tag, msg);
    }

    public void debug(String format, Object... objects) {
        Log.d(tag, String.format(format, objects));
    }

    public void info(String msg) {
        Log.i(tag, msg);
    }

    public void info(String format, Object... objects) {
        Log.i(tag, String.format(format, objects));
    }
}
