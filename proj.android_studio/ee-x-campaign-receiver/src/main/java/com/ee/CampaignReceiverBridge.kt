package com.ee

import android.app.Activity
import android.app.Application
import android.os.RemoteException
import com.android.installreferrer.api.InstallReferrerClient
import com.android.installreferrer.api.InstallReferrerStateListener

class CampaignReceiverBridge /* extends BroadcastReceiver */(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?)
    : IPlugin {
    companion object {
        private val kTag = CampaignReceiverBridge::class.java.name
        private const val kPrefix = "CampaignReceiverBridge"
        private const val kInitialize = "${kPrefix}Initialize"
        private const val kOnReceivedLink = "${kPrefix}OnReceivedLink"
    }

    private var _initialized = false
    private var _client: InstallReferrerClient? = null

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        _logger.info("$kTag: constructor begin: end.")
    }

    // FIXME: legacy Java code.
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

    override fun onCreate(activity: Activity) {}
    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {}

    override fun destroy() {
        deregisterHandlers()
        if (!_initialized) {
            return
        }
    }

    private fun registerHandlers() {
        _bridge.registerHandler(kInitialize) {
            initialize()
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kInitialize)
    }

    fun initialize() {
        Thread.runOnMainThread {
            if (_initialized) {
                return@runOnMainThread
            }
            _initialized = true
            val client = InstallReferrerClient.newBuilder(_application).build()
            client.startConnection(object : InstallReferrerStateListener {
                override fun onInstallReferrerSetupFinished(responseCode: Int) {
                    when (responseCode) {
                        InstallReferrerClient.InstallReferrerResponse.OK -> {
                            _logger.debug("$kTag: ${this::onInstallReferrerSetupFinished.name}: OK")
                            try {
                                val response = client.installReferrer
                                val referral = response.installReferrer
                                _logger.debug("$kTag: ${this::onInstallReferrerSetupFinished.name}: referral = $referral")
                                _bridge.callCpp(kOnReceivedLink, referral)
                                client.endConnection()
                            } catch (ex: RemoteException) {
                                ex.printStackTrace()
                            }
                        }
                        InstallReferrerClient.InstallReferrerResponse.FEATURE_NOT_SUPPORTED ->
                            _logger.debug("$kTag: ${this::onInstallReferrerSetupFinished.name}: FEATURE_NOT_SUPPORTED")
                        InstallReferrerClient.InstallReferrerResponse.SERVICE_UNAVAILABLE ->
                            _logger.debug("$kTag: ${this::onInstallReferrerSetupFinished.name}: SERVICE_UNAVAILABLE")
                        else ->
                            _logger.debug("$kTag: ${this::onInstallReferrerSetupFinished.name}: invalid response code")
                    }
                }

                override fun onInstallReferrerServiceDisconnected() {
                    _logger.debug("$kTag: ${this::onInstallReferrerServiceDisconnected.name}")
                }
            })
            _client = client
        }
    }
}