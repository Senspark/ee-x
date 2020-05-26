package com.ee.ads;

import android.graphics.Point;
import android.view.View;
import android.widget.FrameLayout;

import androidx.annotation.NonNull;

import com.ee.core.internal.Utils;

public class ViewHelper {
    private final View _view;

    public ViewHelper(@NonNull View view) {
        _view = view;
    }

    @NonNull
    public Point getPosition() {
        Utils.checkMainThread();
        /*
        int p[] = new int[2];
        view.getLocationInWindow(p);
        return new Point(p[0], p[1]);
        */
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _view.getLayoutParams();
        return new Point(params.leftMargin, params.topMargin);
    }

    public void setPosition(@NonNull Point position) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _view.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        _view.setLayoutParams(params);
    }

    @NonNull
    public Point getSize() {
        Utils.checkMainThread();
        // return new Point(view.getWidth(), view.getHeight());
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _view.getLayoutParams();
        return new Point(params.width, params.height);
    }

    public void setSize(@NonNull Point size) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params = (FrameLayout.LayoutParams) _view.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        _view.setLayoutParams(params);
    }

    public boolean isVisible() {
        Utils.checkMainThread();
        return _view.getVisibility() == View.VISIBLE;
    }

    public void setVisible(boolean visible) {
        Utils.checkMainThread();
        if (visible) {
            _view.setVisibility(View.VISIBLE);
        } else {
            // View.INVISIBLE doesn't trigger a view redraw.
            // Production: load and then call setVisible(true) doesn't show the native ad.
            _view.setVisibility(View.GONE);
        }
    }
}
