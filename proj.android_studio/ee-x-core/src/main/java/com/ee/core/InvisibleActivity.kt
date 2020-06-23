package com.ee.core

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
            _sharedStartHandler = startHandler
            _sharedFinishHandler = finishHandler
            val intent = Intent(_activity, InvisibleActivity::class.java)
            _activity.startActivity(intent)
        }
    }

    companion object {
        private val _logger = Logger(InvisibleActivity::class.java.name)
        private var _sharedStartHandler: StartHandler? = null
        private var _sharedFinishHandler: FinishHandler? = null
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (savedInstanceState == null) {
            _logger.debug("Launching FacebookLoginActivity flow")
            try {
                val handler = _sharedStartHandler
                    ?: throw IllegalArgumentException("Handler is null")
                handler(this)
            } catch (ex: Exception) {
                ex.printStackTrace()
            } finally {
                _sharedStartHandler = null
            }
        } else {
            _logger.debug("Launching FacebookLoginActivity flow from savedInstanceState")
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        val handler = _sharedFinishHandler ?: throw IllegalStateException("Handler is null")
        try {
            handler(requestCode, resultCode, data)
        } catch (ex: Exception) {
            ex.printStackTrace()
        } finally {
            _sharedFinishHandler = null
            finish()
        }
    }
}