package com.ee.ads;

import android.graphics.Point;
import androidx.annotation.NonNull;

public interface IAdView {
    boolean isLoaded();

    void load();

    @NonNull
    Point getPosition();

    void setPosition(@NonNull Point position);

    @NonNull
    Point getSize();

    void setSize(@NonNull Point size);

    boolean isVisible();

    void setVisible(boolean visible);
}