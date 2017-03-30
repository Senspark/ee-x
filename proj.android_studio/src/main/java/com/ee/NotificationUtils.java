package com.ee;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v7.app.NotificationCompat;

/**
 * Created by Zinge on 3/30/17.
 */

public class NotificationUtils {
    private static final Logger _logger = new Logger(NotificationUtils.class.getName());

    /**
     * Shows the given notification with a generated id.
     *
     * @param context      The context to get the notification manager from.
     * @param notification The notification to be displayed.
     */
    static void showNotification(Context context, android.app.Notification notification) {
        // Pick an id that probably won't overlap anything.
        int notificationId = (int) System.currentTimeMillis();

        showNotification(context, notification, notificationId);
    }

    /**
     * Shows the given notification.
     *
     * @param context        The context to get the notification manager from.
     * @param notification   The notification to be displayed.
     * @param notificationId The unique ID of the notification.
     */
    static void showNotification(Context context, android.app.Notification notification,
                                 int notificationId) {
        NotificationManagerCompat manager = NotificationManagerCompat.from(context);
        try {
            manager.notify(notificationId, notification);
        } catch (SecurityException ex) {
            ex.printStackTrace();

            // Some phones throw an exception for unapproved vibration.
            // Disable vibration.
            notification.defaults =
                android.app.Notification.DEFAULT_LIGHTS | android.app.Notification.DEFAULT_SOUND;

            manager.notify(notificationId, notification);
        }
    }

    /**
     * Builds a notification with the specified parameters.
     *
     * @param title       The title of the notification.
     * @param body        The body of the notification.
     * @param clickIntent The intent will be fired when the user clicks on the notification.
     */
    static android.app.Notification buildNotification(Context context, String title, String body,
                                                      PendingIntent clickIntent) {
        NotificationCompat.Builder builder = new NotificationCompat.Builder(context);

        builder
            .setAutoCancel(true)
            .setContentIntent(clickIntent)
            .setContentText(body)
            .setContentTitle(title)
            .setDefaults(android.app.Notification.DEFAULT_ALL)
            .setSmallIcon(getNotificationIcon(context))
            .setTicker(title);

        return builder.build();
    }

    /**
     * http://stackoverflow.com/questions/28387602/notification-bar-icon-turns-white-in-android-5
     * -lollipop
     */
    private static int getNotificationIcon(Context context) {
        boolean useWhiteIcon =
            (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.LOLLIPOP);
        // https://lolsborn.com/2007/11/27/accessing-android-resources-by-name-at-runtime/
        String resourceName = useWhiteIcon ? "icon_silhouette" : "ic_launcher";
        int id =
            context.getResources().getIdentifier(resourceName, "mipmap", context.getPackageName());
        if (id == 0) {
            _logger.error("Could not find resource id for icon name: " + resourceName);
        }
        return id;
    }

    /**
     * Creates a pending intent that will be fired when the user clicks on the notification.
     *
     * @param context  The context.
     * @param activity The activity will be opened when the notification is clicked.
     */
    static PendingIntent createClickIntent(Context context, Class activityClass) {
        // Use FLAG_ACTIVITY_CLEAR_TOP | FLAG_ACTIVITY_SINGLE_TOP
        // to resume exist activity instead of restarting it.
        Intent intent = new Intent(context, activityClass)
            .setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
            .setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);

        return PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
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
        PendingIntent pendingIntent = PendingIntent.getService(context, requestCode, intent, flags);
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        long intervalInMilliseconds = (long) interval * 1000;
        long delayInMilliseconds = (long) delay * 1000;
        long currentTime = System.currentTimeMillis();
        long triggerTime = currentTime + delayInMilliseconds;
        if (interval == 0) {
            alarmManager.set(AlarmManager.RTC_WAKEUP, triggerTime, pendingIntent);
        } else {
            alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, triggerTime, intervalInMilliseconds,
                pendingIntent);
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
        PendingIntent pendingIntent = PendingIntent.getService(context, requestCode, intent,
            PendingIntent.FLAG_UPDATE_CURRENT);
        pendingIntent.cancel();
        AlarmManager manager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        manager.cancel(pendingIntent);
    }
}
