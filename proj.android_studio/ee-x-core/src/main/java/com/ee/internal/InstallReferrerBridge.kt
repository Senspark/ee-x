package com.ee.internal

import android.util.Log
import com.android.installreferrer.api.InstallReferrerClient
import com.android.installreferrer.api.InstallReferrerStateListener
import com.ee.Utils.getCurrentActivity
import kotlinx.coroutines.delay

object InstallReferrerBridge {
    private lateinit var referrerClient: InstallReferrerClient
    private var connected: Boolean = false
    private var cannotConnect: Boolean = false;

    @JvmStatic
    fun onConnect(): InstallReferrerClient {
        if (connected) {
            Log.w("InstallReferrer", "Service connected, do not run onConnect() any more!!!")
            return referrerClient
        }
        var activity = getCurrentActivity()
        if (activity != null) {
            referrerClient = InstallReferrerClient.newBuilder(activity).build()
            referrerClient.startConnection(object : InstallReferrerStateListener {

                override fun onInstallReferrerSetupFinished(responseCode: Int) {
                    when (responseCode) {
                        InstallReferrerClient.InstallReferrerResponse.OK -> {
                            // Connection established.
                            connected = true
                            Log.e("InstallReferrer", "Connection established.")
                        }
                        InstallReferrerClient.InstallReferrerResponse.FEATURE_NOT_SUPPORTED -> {
                            // API not available on the current Play Store app.
                            connected = false
                            Log.e("InstallReferrer", "API not available on the current Play Store app.")
                        }
                        InstallReferrerClient.InstallReferrerResponse.SERVICE_UNAVAILABLE -> {
                            // Connection couldn't be established.
                            connected = false
                            Log.e("InstallReferrer", "Connection couldn't be established.")
                        }
                    }
                }

                override fun onInstallReferrerServiceDisconnected() {
                    // Try to restart the connection on the next request to
                    // Google Play by calling the startConnection() method.
                    connected = false
                    Log.e("InstallReferrer", "Disconnected.")
                }
            })
        }
        return referrerClient
    }

    @JvmStatic
    suspend fun getUrl(): String {
        Log.w("mylog", "get Url")
        var url = ""
        if (!connected) {
            Log.w("mylog", "not connected")
            onConnect()
            delay(2000)
        }
        if(connected) {
            Log.w("mylog", "hmmm.... still .... not connected")
            url = referrerClient.installReferrer.installReferrer;
        }
        Log.w("mylog", "install url: " + url)
        return url;
    }

    @JvmStatic
    suspend fun getClickTime(): Long {
        var time = 0L
        if (!connected) {
            onConnect()
            delay(2000)
        } else {
            time = referrerClient.installReferrer.referrerClickTimestampSeconds;
        }
        return time
    }

    @JvmStatic
    suspend fun getAppInstallTime(): Long {
        var time = 0L
        if (!connected) {
            onConnect()
            delay(2000)
        } else {
            time = referrerClient.installReferrer.installBeginTimestampSeconds;
        }
        return time
    }

    @JvmStatic
    suspend fun getGooglePlayInstantParam(): Boolean {
        var googlePlayInstantParam = false
        if (!connected) {
            onConnect()
            delay(2000)
        } else {
            googlePlayInstantParam = referrerClient.installReferrer.googlePlayInstantParam
        }
        return googlePlayInstantParam
    }

    @JvmStatic
    fun isConnected(): Boolean {
        return connected;
    }
}