package com.ee.store;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.ee.core.PluginProtocol;
import com.ee.core.internal.MessageBridge;
import com.ee.store.util.IabHelper;

import java.util.List;

/**
 * Created by Zinge on 5/16/17.
 */

public class Store implements PluginProtocol {
    private Context   _context;
    private IabHelper _helper;

    public Store(Context context) {
        _context = context;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();
    }

    public void requestProducts(@Nullable final List<String> itemSkus,
                                @Nullable final List<String> subsSkus) {
        ((Activity) _context).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    Store.this._helper.queryInventoryAsync(true, itemSkus, subsSkus, null);
                } catch (IabHelper.IabAsyncInProgressException e) {
                    e.printStackTrace();
                }
            }
        });
    }

    public void restorePurchases() {

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
        return _helper.handleActivityResult(requestCode, responseCode, data);
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }
}
