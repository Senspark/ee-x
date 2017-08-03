package com.ee.ironsrc;

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
import com.ironsource.mediationsdk.IronSource;
import com.ironsource.mediationsdk.logger.IronSourceError;
import com.ironsource.mediationsdk.model.Placement;
import com.ironsource.mediationsdk.sdk.RewardedVideoListener;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class IronSrcAds implements PluginProtocol, RewardedVideoListener {

    private static final String k__IronSrc_initIronSrc         = "k__IronSrc_initIronSrc";
    private static final String k__IronSrc_isAdsReady         = "k__IronSrc_isAdsReady";
    private static final String k__IronSrc_showAds         = "k__IronSrc_showAds";
    
    private static final Logger _logger = new Logger(IronSrcAds.class.getName());
    
    private Activity _context;

    public IronSrcAds(Context context) {
        _logger.debug("constructor begin: context = " + context);
        _context = (Activity) context;
        registerHandlers();
        
        _logger.debug("constructor end.");
    }
    
    @NonNull
    @Override
    public String getPluginName() {
        return "IronSrcAds";
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
                
                initIronSrcAds(GameID);
                return DictionaryUtils.emptyResult();
            }
        }, k__IronSrc_initIronSrc);
        
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
        }, k__IronSrc_showAds);
        
        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;
                
                String PlacementID = (String) dict.get("PlacementID");
                return (isAdsReady(PlacementID)) ? "true" : "false";
            }
        }, k__IronSrc_isAdsReady);
        
        
    }
    
    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();
        
        bridge.deregisterHandler(k__IronSrc_initIronSrc);
        bridge.deregisterHandler(k__IronSrc_showAds);
        bridge.deregisterHandler(k__IronSrc_isAdsReady);
    }
    
    @SuppressWarnings("WeakerAccess")
    public void initIronSrcAds(final @NonNull String GameID)
    {
        IronSource.init(_context, GameID, IronSource.AD_UNIT.REWARDED_VIDEO);

        IronSource.setRewardedVideoListener(this);
    }
    
    @SuppressWarnings("WeakerAccess")
    public void showAds(final @NonNull String PlacementID)
    {
        IronSource.showRewardedVideo(PlacementID);
    }
    
    @SuppressWarnings("WeakerAccess")
    public boolean isAdsReady(final @NonNull String PlacementID) {
        return IronSource.isRewardedVideoAvailable();
    }

    @Override
    public void onRewardedVideoAdOpened() {

    }

    @Override
    public void onRewardedVideoAdClosed() {

    }

    @Override
    public void onRewardedVideoAvailabilityChanged(boolean b) {

    }

    @Override
    public void onRewardedVideoAdStarted() {

    }

    @Override
    public void onRewardedVideoAdEnded() {

    }

    @Override
    public void onRewardedVideoAdRewarded(Placement placement) {
        Map<String, Object> dict = new HashMap<>();
        dict.put("code", 2);
        dict.put("placement", placement.getPlacementName());

        Log.d("EEIronSrc", "ADS finished dict callback " + JsonUtils.convertDictionaryToString(dict));
        MessageBridge.getInstance().callCpp("__IronSrcAds_callback", JsonUtils.convertDictionaryToString(dict));
    }

    @Override
    public void onRewardedVideoAdShowFailed(IronSourceError ironSourceError) {

    }
}
