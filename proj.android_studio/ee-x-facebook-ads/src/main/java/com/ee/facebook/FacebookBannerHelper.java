package com.ee.facebook;

import android.content.res.Resources;
import android.graphics.Point;

import androidx.annotation.NonNull;

import com.ee.core.internal.Utils;
import com.facebook.ads.AdSize;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import static com.google.common.truth.Truth.assertThat;

class FacebookBannerHelper {
    private final Map<Integer, Point> _sizes;

    private static int getWidthInPixels(AdSize adSize) {
        switch (adSize.getWidth()) {
            case 0: // Interstitial.
            case -1: // Normal ads.
                return Resources.getSystem().getDisplayMetrics().widthPixels;
            default: // Deprecated ads.
                return (int) Utils.convertDpToPixel(adSize.getWidth());
        }
    }

    private static int getHeightInPixels(AdSize adSize) {
        switch (adSize.getHeight()) {
            case 0: // Interstitial.
                return Resources.getSystem().getDisplayMetrics().heightPixels;
            default: // Normal ads.
                return (int) Utils.convertDpToPixel(adSize.getHeight());
        }
    }

    @NonNull
    private static Point convertAdSizeToSize(AdSize adSize) {
        int width = getWidthInPixels(adSize);
        int height = getHeightInPixels(adSize);
        return new Point(width, height);
    }

    FacebookBannerHelper() {
        _sizes = new HashMap<>();
        for (int i = 0; i < 5; ++i) {
            AdSize adSize = getAdSize(i);
            Point size = convertAdSizeToSize(adSize);
            _sizes.put(i, size);
        }
    }

    @NonNull
    AdSize getAdSize(int index) {
        if (index == 0) {
            return AdSize.BANNER_HEIGHT_50;
        }
        if (index == 1) {
            return AdSize.BANNER_HEIGHT_90;
        }
        if (index == 2) {
            return AdSize.INTERSTITIAL;
        }
        if (index == 3) {
            return AdSize.RECTANGLE_HEIGHT_250;
        }
        if (index == 4) {
            return AdSize.BANNER_320_50;
        }
        assertThat(false).isTrue();
        return AdSize.BANNER_320_50;
    }

    @NonNull
    Point getSize(int index) {
        return Objects.requireNonNull(_sizes.get(index));
    }
}
