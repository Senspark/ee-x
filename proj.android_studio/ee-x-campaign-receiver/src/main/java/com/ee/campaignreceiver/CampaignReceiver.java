package com.ee.campaignreceiver;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.RemoteException;
import androidx.annotation.NonNull;

import com.android.installreferrer.api.InstallReferrerClient;
import com.android.installreferrer.api.InstallReferrerStateListener;
import com.android.installreferrer.api.ReferrerDetails;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;

public class CampaignReceiver /* extends BroadcastReceiver */ implements InstallReferrerStateListener, PluginProtocol {
    private static final String k__initialize = "CampaignReceiver_initialize";
    private static final String k__onReceivedLink = "CampaignReceiver_onReceivedLink";

    private static final Logger _logger = new Logger(CampaignReceiver.class.getName());

    private Context _context;
    private Activity _activity;
    private boolean _initialized;
    private InstallReferrerClient _mReferrerClient;


    @SuppressWarnings("unused")
    public CampaignReceiver(Context context) {
        super();
        Utils.checkMainThread();
        _context = context;
        _activity = null;
        _initialized = false;

        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                initialize();
                return "";
            }
        }, k__initialize);
    }

//    @Override
//    public void onReceive(Context context, Intent intent) {
//        Bundle extras = intent.getExtras();
//        if (extras != null) {
//            String referral = extras.getString("referrer");
//            _logger.debug("onReceive referral : " + referral);
//
//            MessageBridge bridge = MessageBridge.getInstance();
//            bridge.callCpp(k__onReceivedLink, referral);
//        }
//    }

    public void initialize() {
        _mReferrerClient = InstallReferrerClient.newBuilder(_context).build();
        _mReferrerClient.startConnection(this);
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

    @Override
    public void onInstallReferrerSetupFinished(int responseCode) {
        switch (responseCode) {
            case InstallReferrerClient.InstallReferrerResponse.OK:
                _logger.debug("onInstallReferrerSetupFinished OK");
                try {
                    ReferrerDetails response = _mReferrerClient.getInstallReferrer();

                    String referral = response.getInstallReferrer();
                    _logger.debug("onInstallReferrerSetupFinished referral : " + referral);

                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onReceivedLink, referral);

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
