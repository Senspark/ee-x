package com.ee.core;

import androidx.annotation.NonNull;

public interface AsyncMessageHandler {
    interface Resolver {
        void resolve(@NonNull String message);
    }

    void handle(@NonNull String message, @NonNull Resolver resolver);
}
