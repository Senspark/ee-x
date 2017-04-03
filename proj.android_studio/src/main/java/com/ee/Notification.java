package com.ee;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.internal.DictionaryUtils;
import com.ee.internal.JsonUtils;
import com.ee.internal.MessageBridge;
import com.ee.internal.MessageHandler;

import java.lang.ref.WeakReference;
import java.util.Locale;
import java.util.Map;

/**
 * Created by Zinge on 3/29/17.
 */

public class Notification implements PluginProtocol {
    private static final String k__notification_schedule       = "__notification_schedule";
    private static final String k__notification_unschedule_all = "__notification_unschedule_all";
    private static final String k__notification_unschedule     = "__notification_unschedule";

    private static final Logger _logger = new Logger(Notification.class.getName());

    private WeakReference<Context> _context;

    public Notification(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = new WeakReference<>(context);
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Notification";
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
        deregisterHandlers();
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String ticker = (String) dict.get("ticker");
                String title = (String) dict.get("title");
                String body = (String) dict.get("body");
                Integer delay = (Integer) dict.get("delay");
                Integer interval = (Integer) dict.get("interval");
                Integer tag = (Integer) dict.get("tag");

                schedule(ticker, title, body, delay, interval, tag);
                return DictionaryUtils.emptyResult();
            }
        }, k__notification_schedule);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                unscheduleAll();
                return DictionaryUtils.emptyResult();
            }
        }, k__notification_unschedule_all);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                Integer tag = (Integer) dict.get("tag");

                unschedule(tag);
                return DictionaryUtils.emptyResult();
            }
        }, k__notification_unschedule);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__notification_schedule);
        bridge.deregisterHandler(k__notification_unschedule_all);
        bridge.deregisterHandler(k__notification_unschedule);
    }

    @SuppressWarnings("WeakerAccess")
    public void schedule(@NonNull String ticker, @NonNull String title, @NonNull String body,
                         @NonNull Integer delay, @NonNull Integer interval, @NonNull Integer tag) {
        _logger.debug(String.format(Locale.getDefault(),
            "schedule: title = %s body = %s delay = %d interval = %d tag = %d", title, body, delay,
            interval, tag));
        Intent intent = new Intent(_context.get(), NotificationService.class);
        intent.putExtra("ticker", ticker);
        intent.putExtra("title", title);
        intent.putExtra("body", body);
        intent.putExtra("tag", tag);
        intent.putExtra("className", _context.get().getClass().getName());
        NotificationUtils.scheduleAlarm(_context.get(), intent, tag,
            PendingIntent.FLAG_UPDATE_CURRENT, delay, interval);
    }

    @SuppressWarnings("WeakerAccess")
    public void unscheduleAll() {
        _logger.debug("unscheduleAll: not supported.");
    }

    @SuppressWarnings("WeakerAccess")
    public void unschedule(@NonNull Integer tag) {
        _logger.debug("unschedule: tag = " + tag);
        Intent intent = new Intent(_context.get(), NotificationService.class);
        NotificationUtils.unscheduleAlarm(_context.get(), intent, tag);
    }
}
