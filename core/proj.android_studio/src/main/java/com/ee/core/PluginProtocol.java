package com.ee.core;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

/**
 * Created by Zinge on 6/1/16.
 */
public abstract class PluginProtocol {
    private static final Logger logger = new Logger(PluginProtocol.class.getName());

    private native void _sendCppMessage0(@NonNull String pluginName,
                                         @NonNull String msg,
                                         int callbackId);

    private native void _sendCppMessage1(@NonNull String pluginName,
                                         @NonNull String msg,
                                         @NonNull String tag);

    protected void sendCppMessage(@NonNull String msg, @NonNull Integer callbackId) {
        logger.debug("sendCppMessage: msg = %s callbackId = %d", msg, callbackId);
        _sendCppMessage0(getPluginName(), msg, callbackId);
    }

    protected void sendCppMessage(@NonNull String msg, @NonNull String tag) {
        logger.debug("sendCppMessage: msg = %s tag = %s", msg, tag);
        _sendCppMessage1(getPluginName(), msg, tag);
    }

    protected Context _context;

    public PluginProtocol(Context context) {
        _context = context;
    }

    @NonNull
    public abstract String getPluginName();

    public abstract boolean onActivityResult(int requestCode, int responseCode, Intent data);

    public abstract void onStart();

    public abstract void onStop();

    public abstract void onResume();

    public abstract void onPause();

    public abstract void onDestroy();

    public abstract boolean onBackPressed();
}
