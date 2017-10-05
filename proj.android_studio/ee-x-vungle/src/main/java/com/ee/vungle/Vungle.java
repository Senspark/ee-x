package com.ee.vungle;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Debug;
import android.support.annotation.NonNull;
import android.util.Log;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.DictionaryUtils;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

import com.vungle.publisher.EventListener;
import com.vungle.publisher.VunglePub;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class Vungle implements PluginProtocol {
    private static final String k__VungleAds_initVungleAds = "k__VungleAds_initVungleAds";
    private static final String k__VungleAds_isAdReady  = "k__VungleAds_isAdReady";
    private static final String k__VungleAds_showAds     = "k__VungleAds_showAds";

    private static final Logger _logger = new Logger(VungleAds.class.getName());

    private final VunglePub vunglePub = VunglePub.getInstance();
    private Activity _context;

    private final EventListener vungleListener = new EventListener()
    {
        @Override
        public void onAdStart() {
            // Called before playing an ad
        }

        @Override
        public void onAdEnd(boolean wasSuccessfulView, boolean wasCallToActionClicked) {
            // Called when the user leaves the ad and control is returned to your application
            // if wasSuccessfulView is true, the user watched the ad and should be rewarded
            // (if this was a rewarded ad).
            // if wasCallToActionClicked is true, the user clicked the call to action
            // button in the ad.

            Log.d("EEVungle", "EEVungle  onAdEnd success " + wasSuccessfulView + "click " + wasCallToActionClicked );
            if (wasSuccessfulView)
            {
                Map<String, Object> dict = new HashMap<>();
                dict.put("code", 2);
                dict.put("placement", "rewardVideo");

                MessageBridge
                        .getInstance()
                        .callCpp("__VungleAds_callback", JsonUtils.convertDictionaryToString(dict));
            }
        }

        @Override
        public void onAdPlayableChanged(boolean isAdPlayable) {
            // Called when the playability state changes. if isAdPlayable is true, you can now
            // play an ad.
            // If false, you cannot yet play an ad.
            Log.d("EEVungle", "EEVungle  onAdPlayableChanged " + isAdPlayable);
        }

        @Override
        public void onAdUnavailable(String reason) {
            // Called when VunglePub.playAd() was called, but no ad was available to play
            Log.d("EEVungle", "EEVungle  onAdUnavailable " + reason);
        }

    };

    public VungleAds(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        registerHandlers();

        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "VungleAds";
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
        vunglePub.onResume();
    }

    @Override
    public void onPause() {
        vunglePub.onPause();
    }

    @Override
    public void onDestroy() {
        vunglePub.clearEventListeners();
        deregisterHandlers();
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    @Override
    public boolean onBackPressed() {
        return false;
    }

    private void registerHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String GameID = (String) dict.get("GameID");

                initVungleAds(GameID);
                return DictionaryUtils.emptyResult();
            }
        }, k__VungleAds_initVungleAds);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String PlacementID = (String) dict.get("PlacementID");

                showAds(PlacementID);
                return DictionaryUtils.emptyResult();
            }
        }, k__VungleAds_showAds);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String PlacementID = (String) dict.get("PlacementID");
                return (isAdsReady(PlacementID)) ? "true" : "false";
            }
        }, k__VungleAds_isAdReady);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__VungleAds_initVungleAds);
        bridge.deregisterHandler(k__VungleAds_showAds);
        bridge.deregisterHandler(k__VungleAds_isAdReady);
    }

    @SuppressWarnings("WeakerAccess")
    public void initVungleAds(final @NonNull String gameId) {
        vunglePub.init(_context, gameId);
        vunglePub.setEventListeners(vungleListener);
    }

    @SuppressWarnings("WeakerAccess")
    public void showAds(final @NonNull String placementId) {
        vunglePub.playAd();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isAdsReady(final @NonNull String placementId) {
        return vunglePub.isAdPlayable();
    }
}
