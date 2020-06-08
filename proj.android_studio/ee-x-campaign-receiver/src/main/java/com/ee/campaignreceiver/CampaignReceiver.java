package com.ee.campaignreceiver;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;

import androidx.annotation.NonNull;

import com.android.installreferrer.api.InstallReferrerClient;
import com.android.installreferrer.api.InstallReferrerStateListener;
import com.android.installreferrer.api.ReferrerDetails;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.IPlugin;
import com.ee.core.internal.Utils;

public class CampaignReceiver /* extends BroadcastReceiver */ implements InstallReferrerStateListener, IPlugin {
    private static final String k__initialize = "CampaignReceiver_initialize";
    private static final String k__onReceivedLink = "CampaignReceiver_onReceivedLink";

    private static final Logger _logger = new Logger(CampaignReceiver.class.getName());

    private Context _context;
    private IMessageBridge _bridge;
    private boolean _initialized;
    private InstallReferrerClient _mReferrerClient;

    public CampaignReceiver(@NonNull Context context, @NonNull IMessageBridge bridge) {
        _logger.debug("constructor begin: context = " + context);
        Utils.checkMainThread();
        _context = context;
        _bridge = bridge;
        _initialized = false;
        registerHandlers();
        _logger.debug("constructor begin: end.");
    }

//    @Override
//    public void onReceive(Context context, Intent intent) {
//        Bundle extras = intent.getExtras();
//        if (extras != null) {
//            String referral = extras.getString("referrer");
//            _logger.debug("onReceive referral : " + referral);
//
//            _bridge.callCpp(k__onReceivedLink, referral);
//        }
//    }

    @NonNull
    @Override
    public String getPluginName() {
        return "CampaignReceiver";
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
        _context = null;
        _bridge = null;
        if (!_initialized) {
            return;
        }
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__initialize, message -> {
            initialize();
            return "";
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
    }

    public void initialize() {
        if (_initialized) {
            return;
        }
        _initialized = true;
        _mReferrerClient = InstallReferrerClient.newBuilder(_context).build();
        _mReferrerClient.startConnection(this);
    }

    @Override
    public void onInstallReferrerSetupFinished(int responseCode) {
        switch (responseCode) {
            case InstallReferrerClient.InstallReferrerResponse.OK:
                _logger.debug("onInstallReferrerSetupFinished OK");
                try {
                    ReferrerDetails response = _mReferrerClient.getInstallReferrer();

                    String referral = response.getInstallReferrer();
                    _logger.debug("onInstallReferrerSetupFinished referral : " + referral);
                    _bridge.callCpp(k__onReceivedLink, referral);

                    _mReferrerClient.endConnection();
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
                break;
            case InstallReferrerClient.InstallReferrerResponse.FEATURE_NOT_SUPPORTED:
                _logger.debug("onInstallReferrerSetupFinished FEATURE_NOT_SUPPORTED");
                break;
            case InstallReferrerClient.InstallReferrerResponse.SERVICE_UNAVAILABLE:
                _logger.debug("onInstallReferrerSetupFinished SERVICE_UNAVAILABLE");
                break;
            default:
                _logger.debug("onInstallReferrerSetupFinished not found responseCode");
        }
    }

    @Override
    public void onInstallReferrerServiceDisconnected() {

    }
}
