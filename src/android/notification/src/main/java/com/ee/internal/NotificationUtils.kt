package com.ee.internal

import android.app.AlarmManager
import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.os.Build
import androidx.core.app.NotificationCompat
import com.ee.Logger

/**
 * Created by Zinge on 3/30/17.
 */
object NotificationUtils {
    private val _logger = Logger(NotificationUtils::class.java.name)
    private val channelId = "ee_x_channel_id_01"
    private val channelName = "ee_x_channel_name"

    /**
     * http://stackoverflow.com/questions/28387602/notification-bar-icon-turns-white-in-android-5
     * -lollipop
     */
    private fun getNotificationIcon(context: Context): Int {
        val useWhiteIcon = Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP
        val resourceName = if (useWhiteIcon) "ic_notification" else "ic_launcher"
        val defType = if (useWhiteIcon) "drawable" else "mipmap"
        val resID = context.resources.getIdentifier(resourceName, defType, context.packageName)
        if (resID == 0) {
            _logger.error("Could not find resource id for icon name: $resourceName")
        }
        return resID
    }

    /**
     * Creates a pending intent that will be fired when the user clicks on the notification.
     *
     * @param context  The context.
     * @param activityClass The activity will be opened when the notification is clicked.
     */
    fun createClickIntent(
        context: Context,
        activityClass: Class<*>,
        requestCode: Int
    ): PendingIntent {
        // Use FLAG_ACTIVITY_CLEAR_TOP | FLAG_ACTIVITY_SINGLE_TOP
        // to resume exist activity instead of restarting it.
        val intent = Intent(context, activityClass)
            .setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
            .setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
        return PendingIntent.getActivity(
            context,
            requestCode,
            intent,
            PendingIntent.FLAG_UPDATE_CURRENT
        )
    }

    /**
     * Schedules an alarm.
     *
     * @param context     The context.
     * @param intent      The intent which will be delivered when the alarm fires.
     * @param requestCode The request code of the pending intent (used for canceling later).
     * @param flags       The flags of the pending intent.
     * @param delay       The delay in seconds.
     * @param interval    How long between each alarms.
     */
    @JvmOverloads
    fun scheduleAlarm(
        context: Context, intent: Intent, requestCode: Int, flags: Int, delay: Int,
        interval: Int = 0
    ) {
        val intervalInMilliseconds = interval.toLong() * 1000
        val delayInMilliseconds = delay.toLong() * 1000
        val currentTime = System.currentTimeMillis()
        val triggerTime = currentTime + delayInMilliseconds
        val alarmManager = context.getSystemService(Context.ALARM_SERVICE) as AlarmManager
        val alarmIntent = PendingIntent.getBroadcast(context, requestCode, intent, flags)
        if (interval == 0) {
            alarmManager[AlarmManager.RTC_WAKEUP, triggerTime] = alarmIntent
        } else {
            alarmManager.setRepeating(
                AlarmManager.RTC_WAKEUP,
                triggerTime,
                intervalInMilliseconds,
                alarmIntent
            )
        }
    }

    /**
     * http://stackoverflow.com/questions/11681095/cancel-an-alarmmanager-pendingintent-in
     * -another-pendingintent
     *
     *
     * http://stackoverflow.com/questions/4315611/android-get-all-pendingintents-set-with
     * -alarmmanager
     *
     *
     * Attempts to cancel the pending intent with the specified intent and request code.
     *
     * @param context     The context.
     * @param intent      The intent which was used to schedule the alarm.
     * @param requestCode The request code which was used to schedule the alarm, this is the ID
     * of the pending intent.
     */
    fun unscheduleAlarm(context: Context, intent: Intent, requestCode: Int) {
        val pendingIntent = PendingIntent.getBroadcast(
            context,
            requestCode,
            intent,
            PendingIntent.FLAG_UPDATE_CURRENT
        )
        if (pendingIntent != null) {
            pendingIntent.cancel()
            val manager = context.getSystemService(Context.ALARM_SERVICE) as AlarmManager
            manager.cancel(pendingIntent)
        }
    }

    fun showNotification(
        context: Context,
        ticker: String,
        title: String,
        body: String,
        clickIntent: PendingIntent,
        tag: Int,
        style: Int,
    ) {
        if (style == 1) {
            showCustomNotification(context, ticker, title, body, clickIntent, tag)
        } else {
            showBasicNotification(context, ticker, title, body, clickIntent, tag)
        }
    }

    private fun showBasicNotification(
        context: Context,
        ticker: String,
        title: String,
        body: String,
        clickIntent: PendingIntent,
        tag: Int
    ) {
        val notificationManager = createChannel(context)
        val notificationBuilder = NotificationCompat.Builder(context, channelId)
        notificationBuilder.setAutoCancel(true)
            .setDefaults(Notification.DEFAULT_ALL)
            .setWhen(System.currentTimeMillis())
            .setSmallIcon(getNotificationIcon(context))
            .setTicker(ticker)
            .setContentTitle(title)
            .setContentText(body)
            .setContentIntent(clickIntent)
        notificationManager.notify(tag, notificationBuilder.build())
    }

    private fun createChannel(context: Context): NotificationManager {
        val notificationManager =
            context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val notificationChannel =
                NotificationChannel(channelId, channelName, NotificationManager.IMPORTANCE_HIGH)
            notificationChannel.setShowBadge(true)
            notificationManager.createNotificationChannel(notificationChannel)
        }
        return notificationManager
    }

    private fun showCustomNotification(
        context: Context,
        ticker: String,
        title: String,
        body: String,
        clickIntent: PendingIntent,
        tag: Int
    ) {
        val notificationManager = createChannel(context)
        val helper = com.senspark.android.notification.NotificationHelper(context)
        val notificationBuilder = helper.cocosCreateNotificationBuilder(body)
        notificationBuilder.setAutoCancel(true)
            .setDefaults(Notification.DEFAULT_ALL)
            .setWhen(System.currentTimeMillis())
            .setTicker(ticker)
            .setContentIntent(clickIntent)
        notificationManager.notify(tag, notificationBuilder.build())
    }
}