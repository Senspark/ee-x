package com.ee.internal

import androidx.annotation.AnyThread
import com.ee.IFullScreenAd
import com.ee.IMessageBridge
import com.ee.Utils

/**
 * Created by Zinge on 10/13/17.
 */
class FullScreenAdHelper(private val _bridge: IMessageBridge,
                         private val _ad: IFullScreenAd,
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