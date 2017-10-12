package com.ee.ads;

import android.graphics.Point;
import android.support.annotation.NonNull;

public interface AdViewInterface {
    boolean isLoaded();

    void load();

    @NonNull
    Point getPosition();

    void setPosition(@NonNull Point position);

    @NonNull
    Point getSize();

    void setSize(@NonNull Point size);

    void setVisible(boolean visible);
}