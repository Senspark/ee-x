package com.ee.notification;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import androidx.core.app.NotificationCompat;
import com.ee.core.Logger;

/**
 * Created by Zinge on 3/30/17.
 */

public class NotificationUtils {
    private static final Logger _logger = new Logger(NotificationUtils.class.getName());


    /**
     * http://stackoverflow.com/questions/28387602/notification-bar-icon-turns-white-in-android-5
     * -lollipop
     */
    private static int getNotificationIcon(Context context) {
        boolean useWhiteIcon = (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP);
        String resourceName = useWhiteIcon ? "ic_notification" : "ic_launcher";
        String defType = useWhiteIcon ? "drawable" : "mipmap";
        int resID = context.getResources().getIdentifier(resourceName , defType ,
                context.getPackageName()) ;
        if (resID == 0) {
            _logger.error("Could not find resource id for icon name: " + resourceName);
        }
        return resID;
    }

    /**
     * Creates a pending intent that will be fired when the user clicks on the notification.
     *
     * @param context  The context.
     * @param activityClass The activity will be opened when the notification is clicked.
     */
    static PendingIntent createClickIntent(Context context, Class activityClass, int requestCode) {
        // Use FLAG_ACTIVITY_CLEAR_TOP | FLAG_ACTIVITY_SINGLE_TOP
        // to resume exist activity instead of restarting it.
        Intent intent = new Intent(context, activityClass)
            .setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
            .setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        return PendingIntent.getActivity(context, requestCode, intent, PendingIntent.FLAG_UPDATE_CURRENT);
    }

    /**
     * Schedules an alarm.
     *
     * @param context     The context.
     * @param intent      The intent which will be delivered when the alarm fires.
     * @param requestCode The request code of the pending intent (used for canceling later).
     * @param flags       The flags of the pending intent.
     * @param delay       The delay in seconds.
     */
    static void scheduleAlarm(Context context, Intent intent, int requestCode, int flags,
                              int delay) {
        scheduleAlarm(context, intent, requestCode, flags, delay, 0);
    }

    /**
     * Schedules an alarm with interval.
     *
     * @param context     The context.
     * @param intent      The intent which will be delivered when the alarm fires.
     * @param requestCode The request code of the pending intent (used for canceling later).
     * @param flags       The flags of the pending intent.
     * @param delay       The delay in seconds.
     * @param interval    How long between each alarms.
     */
    static void scheduleAlarm(Context context, Intent intent, int requestCode, int flags, int delay,
                              int interval) {
        long intervalInMilliseconds = (long) interval * 1000;
        long delayInMilliseconds = (long) delay * 1000;
        long currentTime = System.currentTimeMillis();
        long triggerTime = currentTime + delayInMilliseconds;
        AlarmManager alarmMgr = (AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
        PendingIntent alarmIntent = PendingIntent.getBroadcast(context, requestCode, intent, flags);
        if (interval == 0) {
            alarmMgr.set(AlarmManager.RTC_WAKEUP, triggerTime, alarmIntent);
        } else {
            alarmMgr.setRepeating(AlarmManager.RTC_WAKEUP, triggerTime, intervalInMilliseconds,
                    alarmIntent);
        }
    }

    /**
     * http://stackoverflow.com/questions/11681095/cancel-an-alarmmanager-pendingintent-in
     * -another-pendingintent
     * <p>
     * http://stackoverflow.com/questions/4315611/android-get-all-pendingintents-set-with
     * -alarmmanager
     * <p>
     * Attempts to cancel the pending intent with the specified intent and request code.
     *
     * @param context     The context.
     * @param intent      The intent which was used to schedule the alarm.
     * @param requestCode The request code which was used to schedule the alarm, this is the ID
     *                    of the pending intent.
     */
    static void unscheduleAlarm(Context context, Intent intent, int requestCode) {
        PendingIntent pendingIntent = PendingIntent.getBroadcast(context, requestCode, intent,
                PendingIntent.FLAG_UPDATE_CURRENT);
        if (pendingIntent != null) {
            pendingIntent.cancel();
            AlarmManager manager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
            manager.cancel(pendingIntent);
        }
    }


    static void showNotification(Context context, String ticker, String title, String body,
                                 PendingIntent clickIntent, int tag) {
        NotificationManager notificationManager =
                (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        String channelId = "ee_x_channel_id_01";
        String channelName = "ee_x_channel_name";
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel notificationChannel = new NotificationChannel(channelId, channelName,
                    NotificationManager.IMPORTANCE_HIGH);
            notificationChannel.setShowBadge(true);
            notificationManager.createNotificationChannel(notificationChannel);
        }

        NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(context, channelId);
        notificationBuilder.setAutoCancel(true)
                .setDefaults(Notification.DEFAULT_ALL)
                .setWhen(System.currentTimeMillis())
                .setSmallIcon(getNotificationIcon(context))
                .setTicker(ticker)
                .setContentTitle(title)
                .setContentText(body)
                .setContentIntent(clickIntent);

        notificationManager.notify(tag, notificationBuilder.build());
    }
}
