package com.ee.notification

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent

class NotificationReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent) {
        val ticker = intent.getStringExtra("ticker")
        val title = intent.getStringExtra("title")
        val body = intent.getStringExtra("body")
        val tag = intent.getIntExtra("tag", 0)
        val className = intent.getStringExtra("className")
        try {
            val clazz = Class.forName(className)
            val clickIntent = NotificationUtils.createClickIntent(context, clazz, tag)
            NotificationUtils.showNotification(context, ticker, title, body, clickIntent, tag)
        } catch (e: ClassNotFoundException) {
            e.printStackTrace()
        } catch (e: IllegalArgumentException) {
            // Invalid notification (no valid small icon).
            e.printStackTrace()
        } catch (e: Exception) {
            e.printStackTrace()
        }
    }
}