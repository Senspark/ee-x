package com.ee;

import android.app.*;
import android.app.Notification;
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
            "onStartCommand: intent = " + intent + " flags = " + flags + " startId = " + startId);

        String title = intent.getStringExtra("title");
        String body = intent.getStringExtra("body");
        Integer tag = intent.getIntExtra("tag", 0);
        Class clazz = (Class) intent.getSerializableExtra("class");
        PendingIntent clickIntent = NotificationUtils.createClickIntent(this, clazz);
        Notification notification =
            NotificationUtils.buildNotification(this, title, body, clickIntent);
        NotificationUtils.showNotification(this, notification, tag);

        return START_NOT_STICKY;
    }
}
