package com.ee

import android.util.Log

/**
 * Created by enrevol on 4/13/16.
 */
class Logger {
    private val _tag: String
    private var _logLevel = 0

    constructor(tag: String) {
        _tag = tag
        setDebuggable(true)
    }

    constructor(tag: String, debuggable: Boolean) {
        _tag = tag
        setDebuggable(debuggable)
    }

    fun setDebuggable(debuggable: Boolean) {
        _logLevel = if (debuggable) {
            Log.VERBOSE
        } else {
            Log.INFO
        }
    }

    constructor(tag: String, logLevel: Int) {
        _tag = tag
        _logLevel = logLevel
    }

    fun error(message: String) {
        if (_logLevel <= Log.ERROR) {
            Log.e(_tag, message)
        }
    }

    fun error(message: String, th: Throwable) {
        if (_logLevel <= Log.ERROR) {
            Log.e(_tag, message, th)
        }
    }

    fun warn(message: String) {
        if (_logLevel <= Log.WARN) {
            Log.w(_tag, message)
        }
    }

    fun debug(message: String) {
        if (_logLevel <= Log.DEBUG) {
            Log.d(_tag, message)
        }
    }

    fun debug(format: String, vararg objects: Any?) {
        if (_logLevel <= Log.DEBUG) {
            Log.d(_tag, String.format(format, *objects))
        }
    }

    fun info(message: String) {
        if (_logLevel <= Log.INFO) {
            Log.i(_tag, message)
        }
    }

    fun info(format: String, vararg objects: Any?) {
        if (_logLevel <= Log.INFO) {
            Log.i(_tag, String.format(format, *objects))
        }
    }
}