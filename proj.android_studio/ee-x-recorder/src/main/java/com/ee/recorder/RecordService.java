package com.ee.recorder;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Binder;
import android.os.Build;
import android.os.IBinder;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.core.app.NotificationCompat;

import com.ee.core.Logger;

import java.io.IOException;

public class RecordService extends Service {
    class ServiceBinder extends Binder {
        RecordService getServiceSystem() {
            return RecordService.this;
        }
    }

    static String ACTION_START = "start";
    static String ACTION_STOP = "stop";
    static String ACTION_CANCEL = "cancel";

    private static final Logger _logger = new Logger(RecordService.class.getName());

    private ServiceBinder _binder = new ServiceBinder();
    private MediaRecorder _mediaRecorder;
    private MediaProjection _mediaProjection;
    private MediaProjectionManager _mediaProjectionManager;
    private VirtualDisplay _virtualDisplay;
    private boolean _isRecording;
    private int _screenWidth;
    private int _screenHeight;
    private int _screenDensity;

    @Override
    public void onCreate() {
        super.onCreate();
        _logger.debug("onCreate");
        startForeground(101, createNotification());
        if (isLollipop()) {
            _onCreate();
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        _logger.debug("onDestroy");
        stopForeground(true);
        if (isLollipop()) {
            _onDestroy();
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private void _onCreate() {
        // Get screen dimensions.
        DisplayMetrics metrics = new DisplayMetrics();
        WindowManager windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);
        windowManager.getDefaultDisplay().getRealMetrics(metrics);
        _screenWidth = metrics.widthPixels / 2;
        _screenHeight = metrics.heightPixels / 2;
        _screenDensity = metrics.densityDpi;

        // Initialize components.
        _mediaRecorder = new MediaRecorder();
        _mediaProjectionManager = (MediaProjectionManager) getSystemService(Context.MEDIA_PROJECTION_SERVICE);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private void _onDestroy() {
        if (_virtualDisplay != null) {
            _virtualDisplay.release();
            _virtualDisplay = null;
        }
        if (_mediaRecorder != null) {
            _mediaRecorder.reset();
            _mediaRecorder = null;
        }
        if (_mediaProjection != null) {
            _mediaProjection.stop();
            _mediaProjection = null;
        }
    }

    private Notification createNotification() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            String channelId = RecordService.class.getName();
            NotificationChannel channel = new NotificationChannel(channelId, "Record", NotificationManager.IMPORTANCE_NONE);
            NotificationManager notificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
            notificationManager.createNotificationChannel(channel);
            return new NotificationCompat.Builder(this, channelId)
                .setPriority(NotificationManager.IMPORTANCE_MIN)
                .setCategory(Notification.CATEGORY_SERVICE)
                .build();
        }
        return new Notification();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        _logger.debug("onBind: intent = " + intent);
        return _binder;
    }

    private boolean isLollipop() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        _logger.debug("onStartCommand: intent = " + intent);
        if (!isLollipop()) {
            return START_STICKY;
        }
        String action = intent != null ? intent.getAction() : null;
        if (ACTION_START.equals(action)) {
            startRecording(intent);
        }
        if (ACTION_STOP.equals(action)) {
            stopRecording(intent);
        }
        if (ACTION_CANCEL.equals(action)) {
            cancelRecording(intent);
        }
        return START_STICKY;
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private void startRecording(Intent intent) {
        if (_isRecording) {
            return;
        }
        _isRecording = true;

        int responseCode = intent.getIntExtra("responseCode", -1);
        Intent data = intent.getParcelableExtra("data");
        _mediaProjection = _mediaProjectionManager.getMediaProjection(responseCode, data);

        _mediaRecorder.reset();

        // Initialization order.
        // https://developer.android.com/reference/android/media/MediaRecorder
        _mediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);

        _mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);

        _mediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
        _mediaRecorder.setVideoEncodingBitRate(512 * 1000);
        _mediaRecorder.setVideoFrameRate(30);
        _mediaRecorder.setVideoSize(_screenWidth, _screenHeight);
        _mediaRecorder.setCaptureRate(30);

        String path = intent.getStringExtra("path");
        _mediaRecorder.setOutputFile(path);

        try {
            _mediaRecorder.prepare();
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        _virtualDisplay = _mediaProjection.createVirtualDisplay("Recorder",
            _screenWidth, _screenHeight, _screenDensity,
            DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
            _mediaRecorder.getSurface(), null, null);

        _mediaRecorder.start();
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void stopRecording(Intent intent) {
        if (!_isRecording) {
            return;
        }
        _isRecording = false;
        _mediaRecorder.stop();
        _stopRecording();
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void cancelRecording(Intent intent) {
        if (!_isRecording) {
            return;
        }
        _isRecording = false;
        _stopRecording();
    }

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void _stopRecording() {
        _mediaRecorder.reset();
        _virtualDisplay.release();
    }
}
