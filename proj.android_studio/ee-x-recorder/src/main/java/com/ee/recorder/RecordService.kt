package com.ee.recorder

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.MediaRecorder
import android.media.projection.MediaProjection
import android.media.projection.MediaProjectionManager
import android.os.Binder
import android.os.Build
import android.os.IBinder
import android.util.DisplayMetrics
import android.view.WindowManager
import androidx.annotation.RequiresApi
import androidx.core.app.NotificationCompat
import com.ee.core.Logger
import java.io.IOException

class RecordService : Service() {
    internal inner class ServiceBinder : Binder() {
        val serviceSystem: RecordService
            get() = this@RecordService
    }

    companion object {
        private val _logger = Logger(RecordService::class.java.name)

        const val ACTION_START = "start"
        const val ACTION_STOP = "stop"
        const val ACTION_CANCEL = "cancel"
    }

    private val _binder = ServiceBinder()
    private var _mediaRecorder: MediaRecorder? = null
    private var _mediaProjection: MediaProjection? = null
    private var _mediaProjectionManager: MediaProjectionManager? = null
    private var _virtualDisplay: VirtualDisplay? = null
    private var _isRecording = false
    private var _screenWidth = 0
    private var _screenHeight = 0
    private var _screenDensity = 0

    override fun onCreate() {
        super.onCreate()
        _logger.debug(this::onCreate.name)
        startForeground(101, createNotification())
        if (isLollipop) {
            onCreateInternal()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        _logger.debug(this::onDestroy.name)
        stopForeground(true)
        if (isLollipop) {
            onDestroyInternal()
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private fun onCreateInternal() {
        // Get screen dimensions.
        val metrics = DisplayMetrics()
        val windowManager = getSystemService(Context.WINDOW_SERVICE) as WindowManager
        windowManager.defaultDisplay.getRealMetrics(metrics)
        _screenWidth = metrics.widthPixels / 2
        _screenHeight = metrics.heightPixels / 2
        _screenDensity = metrics.densityDpi

        // Initialize components.
        _mediaRecorder = MediaRecorder()
        _mediaProjectionManager = getSystemService(Context.MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private fun onDestroyInternal() {
        _virtualDisplay?.let { virtualDisplay ->
            virtualDisplay.release()
            _virtualDisplay = null
        }
        _mediaRecorder?.let { mediaRecorder ->
            mediaRecorder.reset()
            _mediaRecorder = null
        }
        _mediaProjection?.let { mediaProjection ->
            mediaProjection.stop()
            _mediaProjection = null
        }
    }

    private fun createNotification(): Notification {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channelId = RecordService::class.java.name
            val channel = NotificationChannel(channelId, "Record", NotificationManager.IMPORTANCE_NONE)
            val notificationManager = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            notificationManager.createNotificationChannel(channel)
            return NotificationCompat.Builder(this, channelId)
                .setPriority(NotificationManager.IMPORTANCE_MIN)
                .setCategory(Notification.CATEGORY_SERVICE)
                .build()
        }
        return Notification()
    }

    override fun onBind(intent: Intent): IBinder? {
        _logger.debug("${this::onBind.name}: intent = $intent")
        return _binder
    }

    private val isLollipop: Boolean
        get() = Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP

    override fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int {
        _logger.debug("${this::onStartCommand.name}: intent = $intent")
        if (!isLollipop) {
            return START_STICKY
        }
        val action = intent.action
        if (ACTION_START == action) {
            startRecording(intent)
        }
        if (ACTION_STOP == action) {
            stopRecording(intent)
        }
        if (ACTION_CANCEL == action) {
            cancelRecording(intent)
        }
        return START_STICKY
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private fun startRecording(intent: Intent) {
        val projectionManager = _mediaProjectionManager ?: return
        val recorder = _mediaRecorder ?: return

        val responseCode = intent.getIntExtra("responseCode", -1)
        val data = intent.getParcelableExtra<Intent>("data") ?: return

        if (_isRecording) {
            return
        }
        _isRecording = true

        val projection = projectionManager.getMediaProjection(responseCode, data)
        _mediaProjection = projection

        recorder.reset()

        // Initialization order.
        // https://developer.android.com/reference/android/media/MediaRecorder
        recorder.setVideoSource(MediaRecorder.VideoSource.SURFACE)
        recorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4)
        recorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264)
        recorder.setVideoEncodingBitRate(512 * 1000)
        recorder.setVideoFrameRate(30)
        recorder.setVideoSize(_screenWidth, _screenHeight)
        recorder.setCaptureRate(30.0)
        val path = intent.getStringExtra("path")
        recorder.setOutputFile(path)
        try {
            recorder.prepare()
        } catch (ex: IOException) {
            ex.printStackTrace()
        }
        _virtualDisplay = projection.createVirtualDisplay("Recorder",
            _screenWidth, _screenHeight, _screenDensity,
            DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
            recorder.surface, null, null)
        recorder.start()
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private fun stopRecording(intent: Intent) {
        val recorder = _mediaRecorder ?: return
        if (!_isRecording) {
            return
        }
        _isRecording = false
        recorder.stop()
        stopRecordingInternal()
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private fun cancelRecording(intent: Intent) {
        if (!_isRecording) {
            return
        }
        _isRecording = false
        stopRecordingInternal()
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private fun stopRecordingInternal() {
        _mediaRecorder?.reset()
        _virtualDisplay?.release()
    }
}