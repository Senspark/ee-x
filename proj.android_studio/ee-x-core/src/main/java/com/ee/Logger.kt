package com.ee

import android.util.Log

/**
 * Created by enrevol on 4/13/16.
 */
class Logger @JvmOverloads constructor(
    private val _tag: String,
    private var _logLevel: Int = Log.VERBOSE) : ILogger {
    var logLevel: Int
        get() = _logLevel
        set(value) {
            _logLevel = value
        }

    override fun error(message: String) {
        if (_logLevel <= Log.ERROR) {
            Log.e(_tag, message)
        }
    }

    override fun error(message: String, th: Throwable) {
        if (_logLevel <= Log.ERROR) {
            Log.e(_tag, message, th)
        }
    }

    override fun warn(message: String) {
        if (_logLevel <= Log.WARN) {
            Log.w(_tag, message)
        }
    }

    override fun debug(message: String) {
        if (_logLevel <= Log.DEBUG) {
            Log.d(_tag, message)
        }
    }

    override fun info(message: String) {
        if (_logLevel <= Log.INFO) {
            Log.i(_tag, message)
        }
    }
}