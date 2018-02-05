package com.ee.facebook;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.MediaRecorder;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.share.Sharer;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.model.ShareVideo;
import com.facebook.share.model.ShareVideoContent;
import com.facebook.share.widget.ShareDialog;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import static android.app.Activity.RESULT_OK;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Facebook implements PluginProtocol {

    private static final String k__facebook_share_link_content      = "__facebook_share_link_content";
    private static final String k__facebook_share_photo_content     = "__facebook_share_photo_content";
    private static final String k__facebook_share_video_content     = "__facebook_share_video_content";
    private static final String k__onResult                         = "__facebook_share_onResult";

    private static final String k__facebook_record_screen           = "__facebook_record_screen";
    private static final String k__facebook_stop_record_screen      = "__facebook_stop_record_screen";

    private static final String k__facebook_check_permission_record      = "__facebook_check_permission_record";
    private static final String k__facebook_cancel_record_screen      = "__facebook_cancel_record_screen";

    private static final String k__facebook_get_video_record_url     = "__facebook_get_video_record_url";

    private static final Logger _logger = new Logger(Facebook.class.getName());

    private Context                             _context;
    private Activity                            _activity;

    private static CallbackManager callbackManager;
    private static ShareDialog _shareDialog;
    private static FacebookCallback _callback = new FacebookCallback<Sharer.Result>(){
        @Override
        public void onSuccess(Sharer.Result result) {
            Utils.checkMainThread();

            MessageBridge bridge = MessageBridge.getInstance();
            bridge.callCpp(k__onResult, "true");
        }

        @Override
        public void onCancel() {
            Utils.checkMainThread();
            MessageBridge bridge = MessageBridge.getInstance();
            bridge.callCpp(k__onResult, "false");
        }

        @Override
        public void onError(FacebookException error) {
            Utils.checkMainThread();
            MessageBridge bridge = MessageBridge.getInstance();
            bridge.callCpp(k__onResult, "false");
        }
    };

    public Facebook(Context context) {
        Utils.checkMainThread();
        _context = context;
        _activity = null;

        callbackManager = CallbackManager.Factory.create();
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Facebook";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
        _shareDialog = new ShareDialog(_activity);

        _shareDialog.registerCallback(callbackManager, _callback);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            mMediaRecorder = new MediaRecorder();
            DisplayMetrics metrics = new DisplayMetrics();
            _activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
            mScreenDensity = metrics.densityDpi;
            mProjectionManager = (MediaProjectionManager) _activity.getSystemService
                    (Context.MEDIA_PROJECTION_SERVICE);

            mMediaProjectionCallback = new MediaProjectionCallback();
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
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            if (mMediaProjection != null) {
                mMediaProjection.stop();
                mMediaProjection = null;
            }
        }
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        _context = null;
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        callbackManager.onActivityResult(requestCode, responseCode, data);

        if(!_hasPermissionForRecord)
        {
            return false;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {

            if (requestCode != PERMISSION_CODE || responseCode != RESULT_OK)
            {
                return false;
            }

            initRecorder();
            prepareRecorder();

            mMediaProjection = mProjectionManager.getMediaProjection(responseCode, data);
            mMediaProjection.registerCallback(mMediaProjectionCallback, null);
            mVirtualDisplay = createVirtualDisplay();
            mMediaRecorder.start();
        }

        return false;
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
                shareLinkContent(message);
                return "";
            }
        }, k__facebook_share_link_content);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                sharePhotoContent(message);
                return "";
            }
        }, k__facebook_share_photo_content);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                try {
                    recordScreen();
                } catch (IOException e) {
                    _logger.debug("recordScreen error");
                    e.printStackTrace();
                }
                return "";
            }
        }, k__facebook_record_screen);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                try {
                    stopRecordScreen();
                } catch (Exception e) {
                    _logger.debug("stopRecordScreen error");
                    e.printStackTrace();
                }
                return "";
            }
        }, k__facebook_stop_record_screen);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                try {
                    cancelRecordScreen();
                } catch (Exception e) {
                    _logger.debug("cancelRecordScreen error");
                    e.printStackTrace();
                }
                return "";
            }
        }, k__facebook_cancel_record_screen);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(requestPermissionForRecord());
            }
        }, k__facebook_check_permission_record);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getVideoRecordUrl();
            }
        }, k__facebook_get_video_record_url);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                shareVideoContent(message);
                return "";
            }
        }, k__facebook_share_video_content);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__facebook_share_link_content);
        bridge.deregisterHandler(k__facebook_share_photo_content);
        bridge.deregisterHandler(k__facebook_share_video_content);
        bridge.deregisterHandler(k__facebook_record_screen);
        bridge.deregisterHandler(k__facebook_stop_record_screen);
        bridge.deregisterHandler(k__facebook_cancel_record_screen);
        bridge.deregisterHandler(k__facebook_check_permission_record);

        bridge.deregisterHandler(k__facebook_get_video_record_url);
    }

    @SuppressWarnings("WeakerAccess")
    public void shareLinkContent(@NonNull String url) {

        ShareLinkContent shareContent = new ShareLinkContent.Builder()
                .setContentUrl(Uri.parse(url))
                .build();

        _shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);
    }

    @SuppressWarnings("WeakerAccess")
    public void sharePhotoContent(@NonNull String photoPath) {
        Bitmap image = BitmapFactory.decodeFile(photoPath);
        SharePhoto photo = new SharePhoto.Builder()
                .setBitmap(image)
                .build();
        SharePhotoContent shareContent = new SharePhotoContent.Builder()
                .addPhoto(photo)
                .build();
        _shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);
    }

    @SuppressWarnings("WeakerAccess")
    public void shareVideoContent(@NonNull String videoPath) {
        _logger.debug("video url " + videoPath);

        File video = new File(videoPath);
        Uri videoFileUri = Uri.fromFile(video);
        ShareVideo videoContent = new ShareVideo.Builder()
                .setLocalUrl(videoFileUri)
                .build();
        ShareVideoContent shareContent = new ShareVideoContent.Builder()
                .setVideo(videoContent)
                .build();
        _shareDialog.show(shareContent, ShareDialog.Mode.AUTOMATIC);
    }

    public String getVideoRecordUrl()
    {
        return _recordedFilePath;
    }

    private int PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE = 1;
    private int PERMISSIONS_REQUEST_RECORD_AUDIO = 2;
    private int PERMISSION_CODE = 1;
    private final int DISPLAY_WIDTH = 480;
    private final int DISPLAY_HEIGHT = 640;
    private MediaRecorder mMediaRecorder;

    private String _recordedFilePath = "";
    private int mScreenDensity;
    private MediaProjectionManager mProjectionManager;
    private MediaProjection mMediaProjection;
    private VirtualDisplay mVirtualDisplay;
    private MediaProjectionCallback mMediaProjectionCallback;

    private boolean _hasPermissionForRecord = false;
    private boolean _startedRecord = false;

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    private class MediaProjectionCallback extends MediaProjection.Callback {
        @Override
        public void onStop() {
//            mMediaRecorder.stop();
//            mMediaRecorder.reset();
//            mMediaProjection = null;
//            stopScreenSharing();
        }
    }

    private void stopScreenSharing() {
        if (mVirtualDisplay == null) {
            return;
        }
        mVirtualDisplay.release();
    }

    private boolean requestPermissionForRecord()
    {
        if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP)
        {
            return false;
        }
        if(_hasPermissionForRecord)
        {
            return true;
        }

        if (ContextCompat.checkSelfPermission(_activity,
                Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        {
            if (ActivityCompat.shouldShowRequestPermissionRationale(_activity,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
                Toast.makeText(_activity, "Gold Miner need this permission for record screen feature", Toast.LENGTH_SHORT);
            }
//            else
            {
                ActivityCompat.requestPermissions(_activity,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        PERMISSIONS_REQUEST_WRITE_EXTERNAL_STORAGE);
            }
        }
        else if(ContextCompat.checkSelfPermission(_activity,
                Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED)
        {
            if (ActivityCompat.shouldShowRequestPermissionRationale(_activity,
                    Manifest.permission.RECORD_AUDIO)) {
                Toast.makeText(_activity, "Gold Miner need this permission for record screen feature", Toast.LENGTH_SHORT);
            }
//            else
            {
                ActivityCompat.requestPermissions(_activity,
                        new String[]{Manifest.permission.RECORD_AUDIO},
                        PERMISSIONS_REQUEST_RECORD_AUDIO);
            }
        }
        else
        {
            _hasPermissionForRecord = true;
        }

        return _hasPermissionForRecord;
    }
    private void cancelRecordScreen() {
        if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP)
        {
            return;
        }
        if (!_hasPermissionForRecord) {
            return;
        }

        mMediaRecorder.reset();
        stopScreenSharing();
    }

    private void stopRecordScreen()
    {
        if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP)
        {
            return;
        }
        if(!_hasPermissionForRecord)
        {
            return;
        }

        mMediaRecorder.stop();
        mMediaRecorder.reset();

        stopScreenSharing();
        _logger.debug("stopRecordScreen");
    }

    private void recordScreen() throws IOException {
        if (android.os.Build.VERSION.SDK_INT < android.os.Build.VERSION_CODES.LOLLIPOP)
        {
            return;
        }
        if(!_hasPermissionForRecord)
        {
            return;
        }

        if (mMediaProjection == null) {
            _activity.startActivityForResult(mProjectionManager.createScreenCaptureIntent(), PERMISSION_CODE);
            return;
        }

        mMediaRecorder.reset();
        _recordedFilePath = "";

        initRecorder();
        prepareRecorder();
        mVirtualDisplay = createVirtualDisplay();
        mMediaRecorder.start();
        _logger.debug("recordScreen");
    }



    private void prepareRecorder() {
        try {
            _logger.debug("prepareRecorder");
            mMediaRecorder.prepare();
            _logger.debug("prepareRecorder end");
        } catch (IllegalStateException | IOException e) {
            e.printStackTrace();
            _logger.debug("prepareRecorder error");
        }
    }

    public String getFilePath() {
        final String directory = Environment.getExternalStorageDirectory() + File.separator + "Recordings";
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

    public String getCurSysDate() {
        return new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss").format(new Date());
    }

    private void initRecorder() {

        _logger.debug("initRecorder");

        mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
        mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.SURFACE);
        mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
        mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
        mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
        mMediaRecorder.setVideoEncodingBitRate(512 * 1000);
        mMediaRecorder.setVideoFrameRate(30);
        mMediaRecorder.setVideoSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        mMediaRecorder.setCaptureRate(30);

        _recordedFilePath = getFilePath();
        mMediaRecorder.setOutputFile(_recordedFilePath);
    }

    private VirtualDisplay createVirtualDisplay() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            return mMediaProjection.createVirtualDisplay("AppActivity",
                    DISPLAY_WIDTH, DISPLAY_HEIGHT, mScreenDensity,
                    DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
                    mMediaRecorder.getSurface(), null /*Callbacks*/, null /*Handler*/);
        }
        return null;
    }
}
