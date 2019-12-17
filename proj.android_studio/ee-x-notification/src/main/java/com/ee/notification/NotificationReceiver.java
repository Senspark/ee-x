package com.ee.notification;

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
            NotificationUtils.showNotification(context, ticker, title, body, clickIntent, tag);
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
