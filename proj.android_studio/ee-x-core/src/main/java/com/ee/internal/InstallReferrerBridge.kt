package com.ee.internal

import android.util.Log
import com.android.installreferrer.api.InstallReferrerClient
import com.android.installreferrer.api.InstallReferrerStateListener
import com.ee.PluginManager
import kotlinx.coroutines.delay

object InstallReferrerBridge {
    private var referrerClient: InstallReferrerClient? = null

    private fun onConnect(): InstallReferrerClient {
        referrerClient?.let {
            return@onConnect it
        }

        var activity = PluginManager.getInstance().getActivity()
        if (activity != null) {
            referrerClient = InstallReferrerClient.newBuilder(activity).build()
            referrerClient?.startConnection(object : InstallReferrerStateListener {

                override fun onInstallReferrerSetupFinished(responseCode: Int) {
                    when (responseCode) {
                        InstallReferrerClient.InstallReferrerResponse.OK -> {
                            // Connection established.
                            Log.w("InstallReferrer", "Connection established.")
                        }
                        InstallReferrerClient.InstallReferrerResponse.FEATURE_NOT_SUPPORTED -> {
                            // API not available on the current Play Store app.
                            Log.w("InstallReferrer", "API not available on the current Play Store app.")
                        }
                        InstallReferrerClient.InstallReferrerResponse.SERVICE_UNAVAILABLE -> {
                            // Connection couldn't be established.
                            Log.w("InstallReferrer", "Connection couldn't be established.")
                        }
                    }
                }

                override fun onInstallReferrerServiceDisconnected() {
                    // Try to restart the connection on the next request to
                    // Google Play by calling the startConnection() method.
                    Log.w("InstallReferrer", "Disconnected.")
                }
            })
        }
        return referrerClient ?: throw  IllegalStateException("Install referrer is not connected")
    }

    suspend fun getUrl(): String {
        var url = ""
        if (referrerClient == null) {
            onConnect()
            delay(2000)
        }
        if (referrerClient != null) {
            url = referrerClient!!.installReferrer.installReferrer;
        }
        return url;
    }
}