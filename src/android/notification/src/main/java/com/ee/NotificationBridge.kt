package com.ee

import android.app.Activity
import android.app.Application
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import androidx.annotation.AnyThread
import com.ee.internal.NotificationReceiver
import com.ee.internal.NotificationUtils
import com.ee.internal.deserialize
import kotlinx.serialization.Serializable

/**
 * Created by Zinge on 3/29/17.
 */
class NotificationBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = NotificationBridge::class.java.name
        private const val kPrefix = "NotificationBridge"
        private const val kSchedule = "${kPrefix}Schedule"
        private const val kUnschedule = "${kPrefix}Unschedule"
        private const val kUnscheduleAll = "${kPrefix}UnscheduleAll"
        private const val kClearAll = "${kPrefix}ClearAll"
    }

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor end")
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

    override fun destroy() {
        deregisterHandlers()
    }

    @Serializable
    private class ScheduleRequest(
        val title: String,
        val ticker: String,
        val body: String,
        val delay: Int,
        val interval: Int,
        val tag: Int
    )

    @Serializable
    private class UnscheduleRequest(
        val tag: Int
    )

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kSchedule) { message ->
            val request = deserialize<ScheduleRequest>(message)
            schedule(request.ticker, request.title, request.body, request.delay, request.interval, request.tag)
            ""
        }
        _bridge.registerHandler(kUnscheduleAll) {
            unscheduleAll()
            ""
        }
        _bridge.registerHandler(kUnschedule) { message ->
            val request = deserialize<UnscheduleRequest>(message)
            unschedule(request.tag)
            ""
        }
        _bridge.registerHandler(kClearAll) {
            clearAll()
            ""
        }
    }

    @AnyThread
    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kSchedule)
        _bridge.deregisterHandler(kUnscheduleAll)
        _bridge.deregisterHandler(kUnschedule)
        _bridge.deregisterHandler(kClearAll)
    }

    fun schedule(ticker: String, title: String, body: String, delay: Int, interval: Int, tag: Int) {
        val activity = _activity ?: return
        val intent = Intent(_application, NotificationReceiver::class.java)
        intent.putExtra("ticker", ticker)
        intent.putExtra("title", title)
        intent.putExtra("body", body)
        intent.putExtra("tag", tag)
        intent.putExtra("className", activity::class.java.name)
        NotificationUtils.scheduleAlarm(_application, intent, tag, PendingIntent.FLAG_UPDATE_CURRENT,
            delay, interval)
    }

    fun unschedule(tag: Int) {
        _logger.debug("$kTag: ${this::unschedule.name}: tag = $tag")
        val intent = Intent(_application, NotificationReceiver::class.java)
        NotificationUtils.unscheduleAlarm(_application, intent, tag)
    }

    fun unscheduleAll() {
        _logger.debug("$kTag: ${this::unscheduleAll.name}: not supported.")
    }

    fun clearAll() {
        val manager = _application.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        manager.cancelAll()
    }
}