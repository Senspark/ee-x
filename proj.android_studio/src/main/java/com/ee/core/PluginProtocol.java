package com.ee.core;

import android.content.Intent;
import android.support.annotation.NonNull;

/**
 * Created by Zinge on 6/1/16.
 */
public interface PluginProtocol {
    boolean onActivityResult(int requestCode, int responseCode, Intent data);

    @NonNull
    String getPluginName();

    void onStart();

    void onStop();

    void onResume();

    void onPause();

    void onDestroy();

    boolean onBackPressed();
}
