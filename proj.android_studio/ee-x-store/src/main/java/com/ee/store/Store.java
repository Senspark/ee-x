package com.ee.store;

import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;

/**
 * Created by Zinge on 5/16/17.
 */

public class Store implements PluginProtocol {
    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Store";
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
}
