package com.ee;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;

/**
 * Created by Zinge on 3/29/17.
 */

public class NotificationService extends Service {
    private static final Logger _logger = new Logger(NotificationService.class.getName());

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        _logger.debug(
            "onStartCommand: intent = " + intent + " extras = " + intent.getExtras() + " flags = " +
            flags + " startId = " + startId);

        String ticker = intent.getStringExtra("ticker");
        String title = intent.getStringExtra("title");
        String body = intent.getStringExtra("body");
        Integer tag = intent.getIntExtra("tag", 0);
        String className = intent.getStringExtra("className");
        try {
            Class clazz = Class.forName(className);
            PendingIntent clickIntent = NotificationUtils.createClickIntent(this, clazz);
            Notification notification =
                NotificationUtils.buildNotification(this, ticker, title, body, clickIntent);
            NotificationUtils.showNotification(this, notification, tag);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }

        return START_NOT_STICKY;
    }
}
