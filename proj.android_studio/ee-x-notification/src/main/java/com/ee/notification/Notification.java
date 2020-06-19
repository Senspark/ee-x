package com.ee.notification;

import android.app.Activity;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.PluginManager;
import com.ee.core.IPlugin;
import com.ee.core.internal.JsonUtils;

import java.util.Locale;
import java.util.Map;

/**
 * Created by Zinge on 3/29/17.
 */

public class Notification implements IPlugin {
    private static final String k__notification_schedule = "__notification_schedule";
    private static final String k__notification_unschedule_all = "__notification_unschedule_all";
    private static final String k__notification_unschedule = "__notification_unschedule";
    private static final String k__notification_clear_all = "__notification_clear_all";

    private static final Logger _logger = new Logger(Notification.class.getName());

    private Context _context;
    private IMessageBridge _bridge;

    public Notification(@NonNull Context context, @NonNull IMessageBridge bridge) {
        _logger.debug("constructor begin: context = " + context);
        _context = context;
        _bridge = bridge;
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
    }

    @Override
    public void destroy() {
        deregisterHandlers();
        _context = null;
        _bridge = null;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__notification_schedule, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assert dict != null;

            String title = (String) dict.get("title");
            String ticker = (String) dict.get("ticker");
            String body = (String) dict.get("body");
            Integer delay = (Integer) dict.get("delay");
            Integer interval = (Integer) dict.get("interval");
            Integer tag = (Integer) dict.get("tag");

            schedule(ticker, title, body, delay, interval, tag);
            return "";
        });

        _bridge.registerHandler(k__notification_unschedule_all, message -> {
            unscheduleAll();
            return "";
        });

        _bridge.registerHandler(k__notification_unschedule, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assert dict != null;

            Integer tag = (Integer) dict.get("tag");
            unschedule(tag);
            return "";
        });

        _bridge.registerHandler(k__notification_clear_all, message -> {
            clearAll();
            return "";
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__notification_schedule);
        _bridge.deregisterHandler(k__notification_unschedule_all);
        _bridge.deregisterHandler(k__notification_unschedule);
        _bridge.deregisterHandler(k__notification_clear_all);
    }

    @SuppressWarnings("WeakerAccess")
    public void schedule(@NonNull String ticker, @NonNull String title, @NonNull String body,
                         @NonNull Integer delay, @NonNull Integer interval, @NonNull Integer tag) {
        _logger.debug(String.format(Locale.getDefault(),
            "schedule: title = %s body = %s delay = %d interval = %d tag = %d", title, body, delay,
            interval, tag));
        Activity activity = PluginManager.getInstance().getActivity();
        String className = "";
        if (activity != null) {
            className = activity.getClass().getName();
        }

        Intent intent = new Intent(_context, NotificationReceiver.class);
        intent.putExtra("ticker", ticker);
        intent.putExtra("title", title);
        intent.putExtra("body", body);
        intent.putExtra("tag", tag);
        intent.putExtra("className", className);
        NotificationUtils.scheduleAlarm(_context, intent, tag, PendingIntent.FLAG_UPDATE_CURRENT,
            delay, interval);
    }

    @SuppressWarnings("WeakerAccess")
    public void unscheduleAll() {
        _logger.debug("unscheduleAll: not supported.");
    }

    @SuppressWarnings("WeakerAccess")
    public void unschedule(@NonNull Integer tag) {
        _logger.debug("unschedule: tag = " + tag);
        Intent intent = new Intent(_context, NotificationReceiver.class);
        NotificationUtils.unscheduleAlarm(_context, intent, tag);
    }

    @SuppressWarnings("WeakerAccess")
    public void clearAll() {
        NotificationManager manager =
            (NotificationManager) _context.getSystemService(Context.NOTIFICATION_SERVICE);
        manager.cancelAll();
    }
}
