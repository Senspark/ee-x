package com.ee.ads;

import androidx.annotation.NonNull;

public class MessageHelper {
    private String _prefix;
    private String _adId;

    public MessageHelper(@NonNull String prefix, @NonNull String adId) {
        _prefix = prefix;
        _adId = adId;
    }

    @NonNull
    public String isLoaded() {
        return _prefix + "_isLoaded_" + _adId;
    }

    @NonNull
    public String load() {
        return _prefix + "_load_" + _adId;
    }

    @NonNull
    public String onLoaded() {
        return _prefix + "_onLoaded_" + _adId;
    }

    @NonNull
    public String onFailedToLoad() {
        return _prefix + "_onFailedToLoad_" + _adId;
    }

    @NonNull
    public String show() {
        return _prefix + "_show_" + _adId;
    }

    @NonNull
    public String onFailedToShow() {
        return _prefix + "_onFailedToShow_" + _adId;
    }

    @NonNull
    public String onClicked() {
        return _prefix + "_onClicked_" + _adId;
    }

    @NonNull
    public String onClosed() {
        return _prefix + "_onClosed_" + _adId;
    }

    @NonNull
    public String getPosition() {
        return _prefix + "_getPosition_" + _adId;
    }

    @NonNull
    public String setPosition() {
        return _prefix + "_setPosition_" + _adId;
    }

    @NonNull
    public String getSize() {
        return _prefix + "_getSize_" + _adId;
    }

    @NonNull
    public String setSize() {
        return _prefix + "_setSize_" + _adId;
    }

    @NonNull
    public String isVisible() {
        return _prefix + "_isVisible_" + _adId;
    }

    @NonNull
    public String setVisible() {
        return _prefix + "_setVisible_" + _adId;
    }

    @NonNull
    public String createInternalAd() {
        return _prefix + "_createInternalAd_" + _adId;
    }

    @NonNull
    public String destroyInternalAd() {
        return _prefix + "_destroyInternalAd_" + _adId;
    }
}
