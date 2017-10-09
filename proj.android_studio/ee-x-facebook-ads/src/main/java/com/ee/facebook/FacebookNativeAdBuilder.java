package com.ee.facebook;

import android.support.annotation.NonNull;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookNativeAdBuilder {
    String adId;
    String layoutName;
    String icon;
    String title;
    String media;
    String socialContext;
    String adChoices;
    String body;
    String action;

    FacebookNativeAdBuilder setAdId(@NonNull String value) {
        adId = value;
        return this;
    }

    FacebookNativeAdBuilder setLayoutName(@NonNull String value) {
        layoutName = value;
        return this;
    }

    FacebookNativeAdBuilder setIcon(@NonNull String key) {
        icon = key;
        return this;
    }

    FacebookNativeAdBuilder setTitle(@NonNull String key) {
        title = key;
        return this;
    }

    FacebookNativeAdBuilder setMedia(@NonNull String key) {
        media = key;
        return this;
    }

    FacebookNativeAdBuilder setSocialContext(@NonNull String key) {
        socialContext = key;
        return this;
    }

    FacebookNativeAdBuilder setAdChoices(@NonNull String key) {
        adChoices = key;
        return this;
    }

    FacebookNativeAdBuilder setBody(@NonNull String key) {
        body = key;
        return this;
    }

    FacebookNativeAdBuilder setAction(@NonNull String key) {
        action = key;
        return this;
    }
}
