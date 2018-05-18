package com.ee.campaignreceiver;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;


public class CampaignReceiver  extends BroadcastReceiver implements PluginProtocol {
    private static final String k__onReceivedLink           = "CampaignReceiver_onReceivedLink";

    private static final Logger _logger = new Logger(CampaignReceiver.class.getName());

    private Activity _activity;
    private boolean  _initialized;

    @SuppressWarnings("unused")
    public CampaignReceiver() {
        super();
        Utils.checkMainThread();
        _activity = null;
        _initialized = false;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        Bundle extras = intent.getExtras();
        if (extras != null) {
            String referral = extras.getString("referrer");
            _logger.debug("onReceive referral : " + referral);

            MessageBridge bridge = MessageBridge.getInstance();
            bridge.callCpp(k__onReceivedLink, referral);
        }
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "CampaignReceiver";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
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
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        if (!_initialized) {
            return;
        }
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