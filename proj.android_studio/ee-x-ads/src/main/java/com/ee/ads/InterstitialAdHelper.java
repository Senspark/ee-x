package com.ee.ads;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.internal.Utils;

/**
 * Created by Zinge on 10/13/17.
 */

public class InterstitialAdHelper {
    private IMessageBridge _bridge;
    private IInterstitialAd _ad;
    private MessageHelper _helper;

    public InterstitialAdHelper(@NonNull IMessageBridge bridge,
                                @NonNull IInterstitialAd ad,
                                @NonNull MessageHelper helper) {
        _bridge = bridge;
        _ad = ad;
        _helper = helper;
    }

    public void registerHandlers() {
        _bridge.registerHandler(message ->
            Utils.toString(_ad.isLoaded()), _helper.isLoaded());

        _bridge.registerHandler(message -> {
            _ad.load();
            return "";
        }, _helper.load());

        _bridge.registerHandler(message -> {
            _ad.show();
            return "";
        }, _helper.show());
    }

    public void deregisterHandlers() {
        _bridge.deregisterHandler(_helper.isLoaded());
        _bridge.deregisterHandler(_helper.load());
        _bridge.deregisterHandler(_helper.show());
    }
}
