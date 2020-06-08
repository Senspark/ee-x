package com.ee.ads;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.Utils;

/**
 * Created by Zinge on 10/13/17.
 */

public class InterstitialAdHelper {
    private final IMessageBridge _bridge;
    private final IInterstitialAd _ad;
    private final MessageHelper _helper;

    public InterstitialAdHelper(@NonNull IMessageBridge bridge,
                                @NonNull IInterstitialAd ad,
                                @NonNull MessageHelper helper) {
        _bridge = bridge;
        _ad = ad;
        _helper = helper;
    }

    public void registerHandlers() {
        _bridge.registerHandler(_helper.isLoaded(), message ->
            Utils.toString(_ad.isLoaded()));

        _bridge.registerHandler(_helper.load(), message -> {
            _ad.load();
            return "";
        });

        _bridge.registerHandler(_helper.show(), message -> {
            _ad.show();
            return "";
        });
    }

    public void deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded());
        _bridge.deregisterHandler(_helper.load());
        _bridge.deregisterHandler(_helper.show());
    }
}
