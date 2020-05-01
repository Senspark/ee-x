package com.ee.admob;

import android.content.Context;
import android.graphics.Point;

import androidx.annotation.NonNull;

import com.google.android.gms.ads.AdSize;

import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import static com.google.common.truth.Truth.assertThat;

class AdMobBannerHelper {
    private Map<Integer, Point> _sizes;

    @NonNull
    private static Point convertAdSizeToSize(Context context, AdSize adSize) {
        int width = adSize.getWidthInPixels(context);
        int height = adSize.getHeightInPixels(context);
        return new Point(width, height);
    }

    AdMobBannerHelper(Context context) {
        _sizes = new HashMap<>();
        for (int i = 0; i < 4; ++i) {
            AdSize adSize = getAdSize(i);
            Point size = convertAdSizeToSize(context, adSize);
            _sizes.put(i, size);
        }
    }

    @NonNull
    AdSize getAdSize(int index) {
        if (index == 0) {
            return AdSize.BANNER;
        }
        if (index == 1) {
            return AdSize.LARGE_BANNER;
        }
        if (index == 2) {
            return AdSize.SMART_BANNER;
        }
        if (index == 3) {
            return AdSize.MEDIUM_RECTANGLE;
        }
        assertThat(false).isTrue();
        return AdSize.INVALID;
    }

    @NonNull
    Point getSize(int index) {
        return Objects.requireNonNull(_sizes.get(index));
    }
}
