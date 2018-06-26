package com.ee.appsflyer;

import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.appsflyer.AppsFlyerLib;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;

import java.util.Map;

public class AppsFlyer implements PluginProtocol {
    private static final String kInitialize      = "AppsFlyerInitialize";
    private static final String kGetDeviceId     = "AppsFlyerGetDeviceId";
    private static final String kSetDebugEnabled = "AppsFlyerSetDebugEnabled";
    private static final String kTrackEvent      = "AppsFlyerTrackEvent";

    private static final Logger _logger = new Logger(AppsFlyer.class.getName());

    private Context        _context;
    private IMessageBridge _bridge;
    private AppsFlyerLib   _tracker;

    public AppsFlyer(Context context) {
        _logger.debug("constructor begin: context = " + context);
        Utils.checkMainThread();
        _context = context;
        _bridge = MessageBridge.getInstance();
        _tracker = AppsFlyerLib.getInstance();
        registerHandlers();
        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "AppsFlyer";
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
        Utils.checkMainThread();
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
        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String devKey = message;
                initialize(devKey);
                return "";
            }
        }, kInitialize);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getDeviceId();
            }
        }, kGetDeviceId);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                setDebugEnabled(Utils.toBoolean(message));
                return "";
            }
        }, kSetDebugEnabled);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;
                String name = (String) dict.get("name");
                Map<String, Object> values = (Map<String, Object>) dict.get("values");
                trackEvent(name, values);
                return "";
            }
        }, kTrackEvent);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _bridge.deregisterHandler(kInitialize);
        _bridge.deregisterHandler(kGetDeviceId);
        _bridge.deregisterHandler(kSetDebugEnabled);
        _bridge.deregisterHandler(kTrackEvent);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String devKey) {
        Utils.checkMainThread();
        _tracker.init(devKey, null, _context.getApplicationContext());
        _tracker.startTracking((Application) _context, devKey);
        _tracker.setCollectAndroidID(true);
        _tracker.setDeviceTrackingDisabled(false);
    }

    @SuppressWarnings("WeakerAccess")
    public String getDeviceId() {
        return _tracker.getAppsFlyerUID(_context.getApplicationContext());
    }

    @SuppressWarnings("WeakerAccess")
    public void setDebugEnabled(boolean enabled) {
        _tracker.setDebugLog(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public void trackEvent(@NonNull String name, @NonNull Map<String, Object> values) {
        _tracker.trackEvent(_context.getApplicationContext(), name, values);
    }
}
