package com.ee.internal

import android.graphics.Point
import androidx.annotation.AnyThread
import com.ee.IAdView
import com.ee.IMessageBridge
import com.ee.Utils
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable

/**
 * Created by Zinge on 10/12/17.
 */
@InternalSerializationApi
class AdViewHelper(private val _bridge: IMessageBridge,
                   private val _view: IAdView,
                   private val _helper: MessageHelper) {
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
            @Serializable
            @Suppress("unused")
            class Response(
                val x: Int,
                val y: Int
            )

            val position = _view.position
            val response = Response(position.x, position.y)
            response.serialize()
        }
        _bridge.registerHandler(_helper.setPosition) { message ->
            @Serializable
            class Request(
                val x: Int,
                val y: Int
            )

            val request = deserialize<Request>(message)
            _view.position = Point(request.x, request.y)
            ""
        }
        _bridge.registerHandler(_helper.getSize) {
            @Serializable
            @Suppress("unused")
            class Response(
                val width: Int,
                val height: Int
            )

            val size = _view.size
            val response = Response(size.x, size.y)
            response.serialize()
        }
        _bridge.registerHandler(_helper.setSize) { message ->
            @Serializable
            class Request(
                val width: Int,
                val height: Int
            )

            val request = deserialize<Request>(message)
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