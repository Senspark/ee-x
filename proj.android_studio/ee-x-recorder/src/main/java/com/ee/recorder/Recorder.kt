package com.ee.recorder

import android.app.Activity
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.media.projection.MediaProjectionManager
import android.os.Build
import android.os.IBinder
import androidx.annotation.AnyThread
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.InvisibleActivity
import com.ee.core.Logger
import com.ee.core.internal.Utils
import com.ee.core.registerHandler
import com.ee.recorder.RecordService.ServiceBinder
import java.text.SimpleDateFormat
import java.util.*

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
class Recorder(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(Recorder::class.java.name)

        private const val k__isSupported = "Recorder_isSupported"
        private const val k__startRecording = "Recorder_startRecording"
        private const val k__stopRecording = "Recorder_stopRecording"
        private const val k__cancelRecording = "Recorder_cancelRecording"
        private const val k__getRecordingUrl = "Recorder_getRecordingUrl"
    }

    private var _recordService: RecordService? = null
    private var _serviceConnection: ServiceConnection? = null
    private var _mediaProjectionManager: MediaProjectionManager? = null
    private var _recordingUrl: String? = null

    init {
        registerHandlers()
        if (isSupported) {
            _mediaProjectionManager = _context.getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        }
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
        if (isSupported) {
            val service = Intent(_activity, RecordService::class.java)
            val connection = object : ServiceConnection {
                override fun onServiceConnected(name: ComponentName, service: IBinder) {
                    val binder = service as ServiceBinder
                    _recordService = binder.serviceSystem
                }

                override fun onServiceDisconnected(name: ComponentName) {
                    _recordService = null
                }
            }
            _serviceConnection = connection
            activity.bindService(service, connection, Context.BIND_AUTO_CREATE)
        }
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}

    override fun onDestroy() {
        _activity = null
        if (isSupported) {
            _serviceConnection = null
        }
    }

    override fun destroy() {
        deregisterHandlers()
    }

    private fun registerHandlers() {
        _bridge.registerHandler(k__isSupported) {
            Utils.toString(isSupported)
        }
        _bridge.registerHandler(k__startRecording) {
            startRecording()
            ""
        }
        _bridge.registerHandler(k__stopRecording) {
            stopRecording()
            ""
        }
        _bridge.registerHandler(k__cancelRecording) {
            cancelRecording()
            ""
        }
        _bridge.registerHandler(k__getRecordingUrl) {
            _recordingUrl ?: ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(k__isSupported)
        _bridge.deregisterHandler(k__startRecording)
        _bridge.deregisterHandler(k__stopRecording)
        _bridge.deregisterHandler(k__cancelRecording)
        _bridge.deregisterHandler(k__getRecordingUrl)
    }

    val isSupported: Boolean
        @AnyThread get() = Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP

    fun startRecording() {
        if (!isSupported) {
            return
        }
        val projectionManager = _mediaProjectionManager ?: return
        val activity = _activity ?: return
        val code = 1
        InvisibleActivity.Builder(activity)
            .onStart { innerActivity ->
                innerActivity.startActivityForResult(projectionManager.createScreenCaptureIntent(), code)
            }
            .onFinish { requestCode, resultCode, data ->
                if (requestCode != code || resultCode != Activity.RESULT_OK) {
                    return@onFinish
                }
                if (!isSupported) {
                    return@onFinish
                }
                _recordingUrl = generateFilePath()

                val intent = Intent(activity, RecordService::class.java)
                intent.action = RecordService.ACTION_START
                intent.putExtra("responseCode", resultCode)
                intent.putExtra("data", data)
                intent.putExtra("path", _recordingUrl)
                activity.startService(intent)
            }
    }

    fun stopRecording() {
        if (!isSupported) {
            return
        }
        val activity = _activity ?: return
        val intent = Intent(activity, RecordService::class.java)
        intent.action = RecordService.ACTION_STOP
        activity.startService(intent)
    }

    private fun cancelRecording() {
        if (!isSupported) {
            return
        }
        val activity = _activity ?: return
        val intent = Intent(activity, RecordService::class.java)
        intent.action = RecordService.ACTION_CANCEL
        activity.startService(intent)
    }

    private fun generateFilePath(): String {
        val activity = _activity ?: return ""
        val formatter = SimpleDateFormat("yyyy-MM-dd-HH-mm-ss", Locale.getDefault())
        val curDate = Date(System.currentTimeMillis())
        val curTime = formatter.format(curDate).replace(" ", "")
        return String.format("%s/capture_%s.mp4", activity.applicationInfo.dataDir, curTime)
    }
}