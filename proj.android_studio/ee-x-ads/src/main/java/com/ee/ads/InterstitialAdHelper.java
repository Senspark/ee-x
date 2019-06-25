package com.ee.ads;

import androidx.annotation.NonNull;

import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;

/**
 * Created by Zinge on 10/13/17.
 */

public class InterstitialAdHelper {
    private String _prefix;
    private String _adId;

    public InterstitialAdHelper(String prefix, String adId) {
        _prefix = prefix;
        _adId = adId;
    }

    @NonNull
    private String k__isLoaded() {
        return _prefix + "_isLoaded_" + _adId;
    }

    @NonNull
    private String k__load() {
        return _prefix + "_load_" + _adId;
    }

    @NonNull
    private String k__show() {
        return _prefix + "_show_" + _adId;
    }

    public void registerHandlers(final IInterstitialAd ad) {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(ad.isLoaded());
            }
        }, k__isLoaded());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                ad.load();
                return "";
            }
        }, k__load());

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(ad.show());
            }
        }, k__show());
    }

    public void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__isLoaded());
        bridge.deregisterHandler(k__load());
        bridge.deregisterHandler(k__show());
    }
}
