package com.ee.notification

import android.app.Activity
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.deserialize
import com.ee.core.registerHandler
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 3/29/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
private class Notification(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(Notification::class.java.name)

        private const val k__notification_schedule = "__notification_schedule"
        private const val k__notification_unschedule_all = "__notification_unschedule_all"
        private const val k__notification_unschedule = "__notification_unschedule"
        private const val k__notification_clear_all = "__notification_clear_all"
    }

    init {
        _logger.debug("constructor begin: context = $_context")
        registerHandlers()
        _logger.debug("constructor end.")
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        _activity = null
    }

    override fun onActivityResult(requestCode: Int, responseCode: Int, data: Intent?): Boolean {
        return false
    }

    override fun destroy() {
        deregisterHandlers()
    }

    private fun registerHandlers() {
        _bridge.registerHandler(k__notification_schedule) { message ->
            @Serializable
            class Request(
                val title: String,
                val ticker: String,
                val body: String,
                val delay: Int,
                val interval: Int,
                val tag: Int
            )

            val request = deserialize<Request>(message)
            schedule(request.ticker, request.title, request.body, request.delay, request.interval, request.tag)
            ""
        }
        _bridge.registerHandler(k__notification_unschedule_all) {
            unscheduleAll()
            ""
        }
        _bridge.registerHandler(k__notification_unschedule) { message ->
            @Serializable
            class Request(
                val tag: Int
            )

            val request = deserialize<Request>(message)
            unschedule(request.tag)
            ""
        }
        _bridge.registerHandler(k__notification_clear_all) {
            clearAll()
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__notification_schedule)
        _bridge.deregisterHandler(k__notification_unschedule_all)
        _bridge.deregisterHandler(k__notification_unschedule)
        _bridge.deregisterHandler(k__notification_clear_all)
    }

    fun schedule(ticker: String, title: String, body: String, delay: Int, interval: Int, tag: Int) {
        val activity = _activity ?: return
        val intent = Intent(_context, NotificationReceiver::class.java)
        intent.putExtra("ticker", ticker)
        intent.putExtra("title", title)
        intent.putExtra("body", body)
        intent.putExtra("tag", tag)
        intent.putExtra("className", activity::class.java.name)
        NotificationUtils.scheduleAlarm(_context, intent, tag, PendingIntent.FLAG_UPDATE_CURRENT,
            delay, interval)
    }

    fun unscheduleAll() {
        _logger.debug("${this::unscheduleAll}: not supported.")
    }

    fun unschedule(tag: Int) {
        _logger.debug("${this::unschedule}: tag = $tag")
        val intent = Intent(_context, NotificationReceiver::class.java)
        NotificationUtils.unscheduleAlarm(_context, intent, tag)
    }

    fun clearAll() {
        val manager = _context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        manager.cancelAll()
    }
}