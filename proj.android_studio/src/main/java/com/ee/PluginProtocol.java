package com.ee.core;

import android.content.Context;
import android.content.Intent;

import java.lang.ref.WeakReference;

/**
 * Created by Zinge on 6/1/16.
 */
public abstract class PluginProtocol {
    private final Logger _logger = new Logger(PluginProtocol.class.getName());

    protected WeakReference<Context> _context;

    public PluginProtocol(Context context) {
        _context = new WeakReference<>(context);
    }

    public abstract boolean onActivityResult(int requestCode, int responseCode, Intent data);

    public abstract void onStart();

    public abstract void onStop();

    public abstract void onResume();

    public abstract void onPause();

    public abstract void onDestroy();

    public abstract boolean onBackPressed();
}
