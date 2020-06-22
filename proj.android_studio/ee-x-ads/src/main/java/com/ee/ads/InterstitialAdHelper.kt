package com.ee.ads

import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.internal.Utils
import com.ee.core.registerHandler

/**
 * Created by Zinge on 10/13/17.
 */
class InterstitialAdHelper(private val _bridge: IMessageBridge,
                           private val _ad: IInterstitialAd,
                           private val _helper: MessageHelper) {
    @AnyThread
    fun registerHandlers() {
        _bridge.registerHandler(_helper.isLoaded) {
            Utils.toString(_ad.isLoaded)
        }
        _bridge.registerHandler(_helper.load) {
            _ad.load()
            ""
        }
        _bridge.registerHandler(_helper.show) {
            _ad.show()
            ""
        }
    }

    @AnyThread
    fun deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded)
        _bridge.deregisterHandler(_helper.load)
        _bridge.deregisterHandler(_helper.show)
    }
}