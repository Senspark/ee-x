package com.ee.unitylib;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.util.Log;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.DictionaryUtils;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;

import java.util.HashMap;
import java.util.Map;

import com.unity3d.ads.IUnityAdsListener;
import com.unity3d.ads.UnityAds;

public class SenUnityAds implements PluginProtocol, IUnityAdsListener {
    private static final String k__unityads_initUnityAds         = "k__unityads_initUnityAds";
    private static final String k__unityads_isAdsReady         = "k__unityads_isAdsReady";
    private static final String k__unityads_showAds         = "k__unityads_showAds";

    private static final Logger _logger = new Logger(UnityAds.class.getName());

    private Activity _context;
    private IUnityAdsListener _listener = this;
    public SenUnityAds(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        registerHandlers();

        _logger.debug("constructor end.");
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "UnityAds";
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
    }

    @Override
    public void onPause() {
    }

    @Override
    public void onDestroy() {
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

                initUnityAds(GameID);
                return DictionaryUtils.emptyResult();
            }
        }, k__unityads_initUnityAds);

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
        }, k__unityads_showAds);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String PlacementID = (String) dict.get("PlacementID");
                return (isAdsReady(PlacementID)) ? "true" : "false";
            }
        }, k__unityads_isAdsReady);


    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__unityads_initUnityAds);
        bridge.deregisterHandler(k__unityads_showAds);
        bridge.deregisterHandler(k__unityads_isAdsReady);
    }

    @SuppressWarnings("WeakerAccess")
    public void initUnityAds(final @NonNull String GameID) {
//        _context.runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                UnityAds.initialize(_context, GameID, _listener);
//            }
//        });

        UnityAds.initialize(_context, GameID, _listener);

    }

    @SuppressWarnings("WeakerAccess")
    public void showAds(final @NonNull String PlacementID) {
//        _context.runOnUiThread(new Runnable() {
//            @Override
//            public void run() {
//                UnityAds.show(_context, PlacementID);
//            }
//        });

        UnityAds.show(_context, PlacementID);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean isAdsReady(final @NonNull String PlacementID) {
        return UnityAds.isReady(PlacementID);
    }

    @Override
    public void onUnityAdsReady(String s) {
        Log.d("EEUNITY", "ADS   ready " + s);
    }

    @Override
    public void onUnityAdsStart(String s) {

    }

    @Override
    public void onUnityAdsFinish(String s, UnityAds.FinishState finishState) {
        Map<String, Object> dict = new HashMap<>();

        Map<UnityAds.FinishState, Integer> states =new HashMap<>();
        states.put(UnityAds.FinishState.ERROR, 0);
        states.put(UnityAds.FinishState.SKIPPED, 1);
        states.put(UnityAds.FinishState.COMPLETED, 2);

        int code = states.get(finishState);
        dict.put("code", code);
        dict.put("placement", s);

        Log.d("EEUNITY", "ADS finished dic callback " + JsonUtils.convertDictionaryToString(dict));
        MessageBridge.getInstance().callCpp("__UnityAds_callback", JsonUtils.convertDictionaryToString(dict));
    }

    @Override
    public void onUnityAdsError(UnityAds.UnityAdsError unityAdsError, String s) {
        Log.d("EEUNITY", "ADS   error " + s);
        Map<String, Object> dict = new HashMap<>();

        dict.put("code", 0);
        dict.put("placement", s);

        Log.d("EEUNITY", "ADS finished dic callback " + JsonUtils.convertDictionaryToString(dict));
        MessageBridge.getInstance().callCpp("__UnityAds_callback", JsonUtils.convertDictionaryToString(dict));
    }
}