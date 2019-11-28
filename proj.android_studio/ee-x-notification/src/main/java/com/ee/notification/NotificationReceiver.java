package com.ee.notification;

import android.app.Notification;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class NotificationReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        String ticker = intent.getStringExtra("ticker");
        String title = intent.getStringExtra("title");
        String body = intent.getStringExtra("body");
        Integer tag = intent.getIntExtra("tag", 0);
        String className = intent.getStringExtra("className");
        try {
            Class clazz = Class.forName(className);
            PendingIntent clickIntent = NotificationUtils.createClickIntent(context, clazz, tag);
            Notification notification =
                    NotificationUtils.buildNotification(context, ticker, title, body, clickIntent);
            NotificationUtils.showNotification(context, notification, tag);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            // Invalid notification (no valid small icon).
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
