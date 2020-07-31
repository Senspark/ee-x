package com.ee

import android.app.Activity
import android.content.Intent
import android.os.Bundle

private typealias StartHandler = (activity: Activity) -> Unit
private typealias FinishHandler = (requestCode: Int, resultCode: Int, data: Intent?) -> Unit

class InvisibleActivity : Activity() {
    class Builder(private val _activity: Activity) {
        private var _startHandler: StartHandler? = null
        private var _finishHandler: FinishHandler? = null

        fun onStart(startHandler: StartHandler): Builder {
            _startHandler = startHandler
            return this
        }

        fun onFinish(finishHandler: FinishHandler): Builder {
            _finishHandler = finishHandler
            return this
        }

        fun process() {
            val startHandler = _startHandler
                ?: throw IllegalArgumentException("StartHandler not set")
            val finishHandler = _finishHandler
                ?: throw IllegalArgumentException("FinishHandler not set")
            Thread.runOnMainThread(Runnable {
                val id = _handleCounter++
                _handlers[id] = Pair(startHandler, finishHandler)
                val intent = Intent(_activity, InvisibleActivity::class.java)
                intent.putExtra("handle_id", id)
                _activity.startActivity(intent)
            })
        }
    }

    companion object {
        private val _logger = Logger(InvisibleActivity::class.java.name)
        private var _handleCounter = 0
        private var _handlers: MutableMap<Int, Pair<StartHandler, FinishHandler>> = HashMap()
    }

    private var _id = -1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        _id = intent.getIntExtra("handle_id", -1)
        if (savedInstanceState == null) {
            _logger.info("onCreate: id = $_id")
            try {
                val handler = _handlers[_id]?.first
                    ?: throw IllegalArgumentException("Handler is null")
                handler(this)
            } catch (ex: Exception) {
                ex.printStackTrace()
                finishAndClear()
            }
        } else {
            _logger.info("onCreate: from saved state, id = $_id")
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        _logger.info("${this::onActivityResult.name}: requestCode = $requestCode resultCode = $resultCode")
        super.onActivityResult(requestCode, resultCode, data)
        val handler = _handlers[_id]?.second
            ?: throw IllegalStateException("Handler is null")
        try {
            handler(requestCode, resultCode, data)
        } catch (ex: Exception) {
            ex.printStackTrace()
        } finally {
            finishAndClear()
        }
    }

    private fun finishAndClear() {
        _logger.info("${this::finishAndClear.name}: id = $_id")
        _handlers.remove(_id)
        finish()
    }
}