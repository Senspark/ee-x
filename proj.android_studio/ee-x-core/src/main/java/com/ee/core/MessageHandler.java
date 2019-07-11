package com.ee.core;

import androidx.annotation.NonNull;

/**
 * Created by Zinge on 3/29/17.
 */
public interface MessageHandler {
    @NonNull
    String handle(@NonNull String message);
}