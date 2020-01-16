package com.ee.recorder;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.util.DisplayMetrics;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Recorder implements PluginProtocol {
    private static final String k__isSupported = "Recorder_isSupported";
    private static final String k__startRecording = "Recorder_startRecording";
    private static final String k__stopRecording = "Recorder_stopRecording";
    private static final String k__cancelRecording = "Recorder_cancelRecording";
    private static final String k__getRecordingUrl = "Recorder_getRecordingUrl";

    private static final Logger _logger = new Logger(Recorder.class.getName());

    private static final int PERMISSION_CODE = 1;

    private Activity _activity;
    private boolean _isRecording;
    private String _filePath;
    private int _screenWidth;
    private int _screenHeight;
    private int _screenDensity;
    private MediaRecorder _mediaRecorder;
    private MediaProjection _mediaProjection;
    private MediaProjectionManager _mediaProjectionManager;
    private VirtualDisplay _virtualDisplay;

    public Recorder() {
        Utils.checkMainThread();
        _activity = null;
        _isRecording = false;
        _mediaRecorder = new MediaRecorder();
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Recorder";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;

        DisplayMetrics metrics = new DisplayMetrics();
        _activity.getWindowManager().getDefaultDisplay().getRealMetrics(metrics);
        _screenDensity = metrics.densityDpi;
        _screenWidth = metrics.widthPixels / 2;
        _screenHeight = metrics.heightPixels / 2;

        if (isSupported()) {
            _mediaProjectionManager = (MediaProjectionManager) _activity.getSystemService(
                Context.MEDIA_PROJECTION_SERVICE);
        }
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
        if (isSupported()) {
            if (_mediaProjection != null) {
                _mediaProjection.stop();
                _mediaProjection = null;
            }
        }
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(isSupported());
            }
        }, k__isSupported);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                startRecording();
                return "";
            }
        }, k__startRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                stopRecording();
                return "";
            }
        }, k__stopRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                cancelRecording();
                return "";
            }
        }, k__cancelRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getRecordingUrl();
            }
        }, k__getRecordingUrl);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__startRecording);
        bridge.deregisterHandler(k__stopRecording);
        bridge.deregisterHandler(k__cancelRecording);
        bridge.deregisterHandler(k__getRecordingUrl);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        if (requestCode != PERMISSION_CODE || responseCode != Activity.RESULT_OK) {
            return false;
        }
        if (!isSupported()) {
            return false;
        }
        _mediaProjection = _mediaProjectionManager.getMediaProjection(responseCode, data);
        _startRecording();
        return true;
    }

    public boolean isSupported() {
        return Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP;
    }

    @SuppressWarnings("WeakerAccess")
    public void startRecording() {
        if (!isSupported()) {
            return;
        }
        if (_mediaProjection == null) {
            _activity.startActivityForResult(_mediaProjectionManager.createScreenCaptureIntent(), PERMISSION_CODE);
            return;
        }
        if (_isRecording) {
            return;
        }
        _mediaRecorder.reset();
        _startRecording();
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private void _startRecording() {
        _filePath = generateFilePath();

        // Initialization order.
        // https://developer.android.com/reference/android/media/MediaRecorder
        _mediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);

        _mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);

        _mediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
        _mediaRecorder.setVideoEncodingBitRate(512 * 1000);
        _mediaRecorder.setVideoFrameRate(30);
        _mediaRecorder.setVideoSize(_screenWidth, _screenHeight);
        _mediaRecorder.setCaptureRate(30);
        _mediaRecorder.setOutputFile(_filePath);

        try {
            _mediaRecorder.prepare();
        } catch (IOException ex) {
            ex.printStackTrace();
        }

        _virtualDisplay = createVirtualDisplay(_mediaProjection,
            _screenWidth, _screenHeight, _screenDensity, _mediaRecorder);

        _mediaRecorder.start();
        _isRecording = true;
    }

    @SuppressWarnings("WeakerAccess")
    public void stopRecording() {
        if (!_isRecording) {
            return;
        }
        if (!isSupported()) {
            return;
        }
        _mediaRecorder.stop();
    }

    @SuppressWarnings("WeakerAccess")
    public void cancelRecording() {
        if (!_isRecording) {
            return;
        }
        if (!isSupported()) {
            return;
        }
        _stopRecording();
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private void _stopRecording() {
        _mediaRecorder.reset();
        _virtualDisplay.release();
        _isRecording = false;
    }

    @SuppressWarnings("WeakerAccess")
    public String getRecordingUrl() {
        return _filePath;
    }

    private String generateFilePath() {
        SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd-HH-mm-ss", Locale.getDefault());
        Date curDate = new Date(System.currentTimeMillis());
        String curTime = formatter.format(curDate).replace(" ", "");
        return String.format("%s/capture_%s.mp4", _activity.getApplicationInfo().dataDir, curTime);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private VirtualDisplay createVirtualDisplay(MediaProjection projection,
                                                int width, int height, int density, MediaRecorder recorder) {
        return projection.createVirtualDisplay("Recorder",
            width, height, density, DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
            recorder.getSurface(), null, null);
    }
}
