package com.ee.core;

import android.app.Activity;
import android.content.Intent;

import androidx.annotation.NonNull;

/**
 * Created by Zinge on 6/1/16.
 */
public interface IPlugin {
    /**
     * Gets the name of the plugin.
     */
    @NonNull
    String getPluginName();

    /**
     * Called when the main activity has been created.
     */
    void onCreate(@NonNull Activity activity);

    /**
     * Called when the main activity has been started.
     */
    void onStart();

    /**
     * Called when the main activity has been stopped.
     */
    void onStop();

    /**
     * Called when the main activity has been resumed.
     */
    void onResume();

    /**
     * Called when the main activity has been paused.
     */
    void onPause();

    /**
     * Called when the main activity has been destroyed.
     */
    void onDestroy();

    /**
     * Called when the application has been destroyed.
     */
    void destroy();

    boolean onActivityResult(int requestCode, int responseCode, Intent data);
}
