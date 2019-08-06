package com.ee.recorder;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.os.Build;
import android.os.Environment;
import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import android.util.DisplayMetrics;

import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.Utils;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import static android.app.Activity.RESULT_OK;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Recorder implements PluginProtocol {
    private static final String k__startScreenRecording = "Recorder_startScreenRecording";
    private static final String k__stopScreenRecording = "Recorder_stopScreenRecording";
    private static final String k__cancelScreenRecording = "Recorder_cancelScreenRecording";
    private static final String k__getScreenRecordingUrl = "Recorder_getScreenRecordingUrl";
    private static final String k__checkRecordingPermission = "Recorder_checkRecordingPermission";

    private static final Logger _logger = new Logger(Recorder.class.getName());

    private static final int PERMISSION_CODE = 1;
    private static final int DISPLAY_WIDTH = 480;
    private static final int DISPLAY_HEIGHT = 640;

    private Activity _activity;
    private MediaRecorder _mediaRecorder;
    private String _recordedFilePath = "";
    private int _screenDensity;
    private MediaProjectionManager _projectionManager;
    private MediaProjection _mediaProjection;
    private VirtualDisplay _virtualDisplay;
    private MediaProjection.Callback _mediaProjectionCallback;

    private boolean _hasRecordingPermission;

    public Recorder() {
        Utils.checkMainThread();
        _activity = null;
        _hasRecordingPermission = false;
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
        _mediaRecorder = new MediaRecorder();

        DisplayMetrics metrics = new DisplayMetrics();
        _activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        _screenDensity = metrics.densityDpi;

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            _projectionManager = (MediaProjectionManager) _activity.getSystemService(Context
                    .MEDIA_PROJECTION_SERVICE);

            _mediaProjectionCallback = new MediaProjection.Callback() {
                public void onStop() {
                    //
                }
            };
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
        if (_mediaProjection != null) {
            _mediaProjection.stop();
            _mediaProjection = null;
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
                startScreenRecording();
                return "";
            }
        }, k__startScreenRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                stopRecordScreen();
                return "";
            }
        }, k__stopScreenRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                cancelScreenRecording();
                return "";
            }
        }, k__cancelScreenRecording);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(checkRecordingPermission());
            }
        }, k__checkRecordingPermission);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getScreenRecordingUrl();
            }
        }, k__getScreenRecordingUrl);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__startScreenRecording);
        bridge.deregisterHandler(k__stopScreenRecording);
        bridge.deregisterHandler(k__cancelScreenRecording);
        bridge.deregisterHandler(k__checkRecordingPermission);
        bridge.deregisterHandler(k__getScreenRecordingUrl);
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        if (!_hasRecordingPermission) {
            return false;
        }
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            return false;
        }
        if (requestCode != PERMISSION_CODE || responseCode != RESULT_OK) {
            return false;
        }
        initRecorder();
        prepareRecorder();

        _mediaProjection = _projectionManager.getMediaProjection(responseCode, data);
        _mediaProjection.registerCallback(_mediaProjectionCallback, null);
        _virtualDisplay = createVirtualDisplay();
        _mediaRecorder.start();
        return true;
    }

    private void stopScreenSharing() {
        if (_virtualDisplay == null) {
            return;
        }
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            return;
        }
        _virtualDisplay.release();
    }

    private void startScreenRecording() {
        if (!_hasRecordingPermission) {
            return;
        }
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP) {
            return;
        }
        if (_mediaProjection == null) {
            _activity.startActivityForResult(_projectionManager.createScreenCaptureIntent(),
                    PERMISSION_CODE);
            return;
        }

        _mediaRecorder.reset();
        _recordedFilePath = "";

        initRecorder();
        prepareRecorder();

        _virtualDisplay = createVirtualDisplay();
        _mediaRecorder.start();
        _logger.debug("startScreenRecording");
    }

    private void stopRecordScreen() {
        if (!_hasRecordingPermission
                || _recordedFilePath.equals("")) {
            return;
        }
        _mediaRecorder.stop();
        _mediaRecorder.reset();

        stopScreenSharing();
        _logger.debug("stopRecordScreen");
    }

    private void cancelScreenRecording() {
        if (!_hasRecordingPermission) {
            return;
        }
        _mediaRecorder.reset();
        stopScreenSharing();
    }

    @SuppressWarnings("WeakerAccess")
    public String getScreenRecordingUrl() {
        return _recordedFilePath;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean checkRecordingPermission() {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.LOLLIPOP
                || _activity == null) {
            return false;
        }

        if (_hasRecordingPermission) {
            return true;
        }

        if (ContextCompat.checkSelfPermission(_activity, Manifest.permission
                .WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED || ContextCompat
                .checkSelfPermission(_activity, Manifest.permission.RECORD_AUDIO) !=
                PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(_activity, new String[]{Manifest.permission
                    .WRITE_EXTERNAL_STORAGE, Manifest.permission.RECORD_AUDIO}, PERMISSION_CODE);
        } else {
            _hasRecordingPermission = true;
        }
        return _hasRecordingPermission;
    }

    private void prepareRecorder() {
        try {
            _logger.debug("prepareRecorder");
            _mediaRecorder.prepare();
            _logger.debug("prepareRecorder end");
        } catch (IllegalStateException | IOException e) {
            e.printStackTrace();
            _logger.debug("prepareRecorder error");
        }
    }

    private String getFilePath() {
        final String directory = Environment.getExternalStorageDirectory() + File.separator +
                "Recordings";
        if (!Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            return null;
        }
        final File folder = new File(directory);
        boolean success = true;
        if (!folder.exists()) {
            success = folder.mkdir();
        }
        String filePath;
        if (success) {
            String videoName = ("capture_" + getCurSysDate() + ".mp4");
            filePath = directory + File.separator + videoName;
        } else {
            return null;
        }
        return filePath;
    }

    private String getCurSysDate() {
        return new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss", Locale.getDefault()).format(new Date());
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private void initRecorder() {
        _logger.debug("initRecorder");

        _mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        _mediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);
        _mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
        _mediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
        _mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
        _mediaRecorder.setVideoEncodingBitRate(512 * 1000);
        _mediaRecorder.setVideoFrameRate(30);
        _mediaRecorder.setVideoSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        _mediaRecorder.setCaptureRate(30);

        _recordedFilePath = getFilePath();
        _mediaRecorder.setOutputFile(_recordedFilePath);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    private VirtualDisplay createVirtualDisplay() {
        return _mediaProjection.createVirtualDisplay("AppActivity", DISPLAY_WIDTH,
                DISPLAY_HEIGHT, _screenDensity, DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                _mediaRecorder.getSurface(), null, null);
    }
}
