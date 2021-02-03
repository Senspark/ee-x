package com.ee

import androidx.annotation.AnyThread
import com.google.android.gms.common.ConnectionResult
import com.google.android.gms.common.GoogleApiAvailability
import com.google.firebase.FirebaseApp
import kotlinx.coroutines.Deferred
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.async
import kotlin.coroutines.resume
import kotlin.coroutines.suspendCoroutine

/**
 * Created by Zinge on 7/4/16.
 */
class FirebaseInitializer private constructor() {
    companion object {
        private val kTag = FirebaseInitializer::class.java.name
        private val _sharedInstance = FirebaseInitializer()

        val instance: FirebaseInitializer
            @AnyThread get() = _sharedInstance
    }

    private val _logger = PluginManager.instance.logger
    private val _scope = MainScope()
    private var _checkGooglePlayServicesTask: Deferred<Boolean>? = null
    private var _initialized = false

    init {
        _logger.info("$kTag: constructor")
    }

    private suspend fun checkGooglePlayServices(): Boolean {
        _checkGooglePlayServicesTask?.let {
            return@checkGooglePlayServices it.await()
        }
        _checkGooglePlayServicesTask = _scope.async {
            suspendCoroutine { cont ->
                _logger.info("$kTag: checkGooglePlayServices")
                Thread.runOnMainThread {
                    val activity = PluginManager.instance.activity
                    if (activity == null) {
                        _logger.warn("$kTag: checkGooglePlayServices: null activity")
                        cont.resume(false)
                        return@runOnMainThread
                    }
                    val api = GoogleApiAvailability.getInstance()
                    val result = api.isGooglePlayServicesAvailable(activity)
                    if (result == ConnectionResult.SUCCESS) {
                        _logger.info("$kTag: checkGooglePlayServices: GooglePlayServices is available")
                        cont.resume(true)
                        return@runOnMainThread
                    } else {
                        _logger.warn("$kTag: checkGooglePlayServices: GooglePlayServices not available")
                        _logger.info("$kTag: checkGooglePlayServices: attempt to fix")
                        api.makeGooglePlayServicesAvailable(activity).addOnCompleteListener { task ->
                            if (task.isSuccessful) {
                                _logger.info("$kTag: checkGooglePlayServices: GooglePlayServices fixed")
                                cont.resume(true)
                            } else {
                                _logger.warn("$kTag: checkGooglePlayServices: failed to fix GooglePlayServices")
                                cont.resume(false)
                            }
                        }
                    }
                }
            }
        }
        _checkGooglePlayServicesTask?.let {
            return@checkGooglePlayServices it.await()
        }
        return false
    }

    private fun initializeApp(): Boolean {
        _logger.info("$kTag: initializeApp")
        if (_initialized) {
            return true
        }
        val application = PluginManager.instance.application
        return if (application == null) {
            _logger.warn("$kTag: initializeApp: null application")
            false
        } else {
            _initialized = true
            FirebaseApp.initializeApp(application)
            _logger.info("$kTag: initializeApp: success")
            true
        }
    }

    public suspend fun initialize(checkGooglePlayServices: Boolean): Boolean {
        if (checkGooglePlayServices) {
            if (checkGooglePlayServices()) {
                // OK.
            } else {
                return false
            }
        }
        return initializeApp()
    }
}