package com.ee

import android.app.Activity
import android.app.Application
import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.ServiceConnection
import android.media.projection.MediaProjectionManager
import android.os.Build
import android.os.IBinder
import androidx.annotation.AnyThread
import androidx.annotation.RequiresApi
import com.ee.internal.RecordService
import com.ee.internal.RecordService.ServiceBinder
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
@RequiresApi(Build.VERSION_CODES.LOLLIPOP)
class RecorderBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _application: Application,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val kTag = RecorderBridge::class.java.name
        private const val kPrefix = "RecorderBridge"
        private const val kIsSupported = "${kPrefix}IsSupported"
        private const val kStartRecording = "${kPrefix}StartRecording"
        private const val kStopRecording = "${kPrefix}StopRecording"
        private const val kCancelRecording = "${kPrefix}CancelRecording"
        private const val kGetRecordingUrl = "${kPrefix}GetRecordingUrl"
    }

    private var _recordService: RecordService? = null
    private var _serviceConnection: ServiceConnection? = null
    private var _mediaProjectionManager: MediaProjectionManager? = null
    private var _recordingUrl: String? = null

    init {
        _logger.info("$kTag: constructor begin: application = $_application activity = $_activity")
        registerHandlers()
        if (isSupported) {
            _mediaProjectionManager = _application.getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        }
        _logger.info("$kTag: constructor end.")
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
        _bridge.registerHandler(kIsSupported) {
            Utils.toString(isSupported)
        }
        _bridge.registerHandler(kStartRecording) {
            startRecording()
            ""
        }
        _bridge.registerHandler(kStopRecording) {
            stopRecording()
            ""
        }
        _bridge.registerHandler(kCancelRecording) {
            cancelRecording()
            ""
        }
        _bridge.registerHandler(kGetRecordingUrl) {
            _recordingUrl ?: ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kIsSupported)
        _bridge.deregisterHandler(kStartRecording)
        _bridge.deregisterHandler(kStopRecording)
        _bridge.deregisterHandler(kCancelRecording)
        _bridge.deregisterHandler(kGetRecordingUrl)
    }

    private val isSupported: Boolean
        @AnyThread get() = Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP

    private fun startRecording() {
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

    private fun stopRecording() {
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