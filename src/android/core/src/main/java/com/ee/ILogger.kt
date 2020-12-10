package com.ee

interface ILogger {
    fun error(message: String)
    fun error(message: String, th: Throwable)
    fun warn(message: String)
    fun debug(message: String)
    fun info(message: String)
}