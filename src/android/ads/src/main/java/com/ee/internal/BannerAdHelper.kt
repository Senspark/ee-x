package com.ee.internal

import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.IBannerAd
import com.ee.IMessageBridge
import com.ee.Utils
import kotlinx.serialization.Serializable

/**
 * Created by Zinge on 10/12/17.
 */
class BannerAdHelper(private val _bridge: IMessageBridge,
                     private val _view: IBannerAd,
                     private val _helper: MessageHelper) {
    @Serializable
    @Suppress("unused")
    private class GetPositionResponse(
        val x: Int,
        val y: Int
    )

    @Serializable
    private class SetPositionRequest(
        val x: Int,
        val y: Int
    )

    @Serializable
    @Suppress("unused")
    private class GetSizeResponse(
        val width: Int,
        val height: Int
    )

    @Serializable
    private class SetSizeRequest(
        val width: Int,
        val height: Int
    )

    @AnyThread
    fun registerHandlers() {
        _bridge.registerHandler(_helper.isLoaded) {
            Utils.toString(_view.isLoaded)
        }
        _bridge.registerHandler(_helper.load) {
            _view.load()
            ""
        }
        _bridge.registerHandler(_helper.getPosition) {
            val position = _view.position
            val response = GetPositionResponse(position.x, position.y)
            response.serialize()
        }
        _bridge.registerHandler(_helper.setPosition) { message ->
            val request = deserialize<SetPositionRequest>(message)
            _view.position = Point(request.x, request.y)
            ""
        }
        _bridge.registerHandler(_helper.getSize) {
            val size = _view.size
            val response = GetSizeResponse(size.x, size.y)
            response.serialize()
        }
        _bridge.registerHandler(_helper.setSize) { message ->
            val request = deserialize<SetSizeRequest>(message)
            _view.size = Point(request.width, request.height)
            ""
        }
        _bridge.registerHandler(_helper.isVisible) {
            Utils.toString(_view.isVisible)
        }
        _bridge.registerHandler(_helper.setVisible) { message ->
            _view.isVisible = Utils.toBoolean(message)
            ""
        }
    }

    @AnyThread
    fun deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded)
        _bridge.deregisterHandler(_helper.load)
        _bridge.deregisterHandler(_helper.getPosition)
        _bridge.deregisterHandler(_helper.setPosition)
        _bridge.deregisterHandler(_helper.getSize)
        _bridge.deregisterHandler(_helper.setSize)
        _bridge.deregisterHandler(_helper.isVisible)
        _bridge.deregisterHandler(_helper.setVisible)
    }
}