package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Zinge on 10/13/17.
 */

public class AdMob implements PluginProtocol, RewardedVideoAdListener {
    private static final String k__createBannerAd        = "AdMob_createBannerAd";
    private static final String k__destroyBannerAd       = "AdMob_destroyBannerAd";
    private static final String k__createNativeAd        = "AdMob_createNativeAd";
    private static final String k__destroyNativeAd       = "AdMob_destroyNativeAd";
    private static final String k__createInterstitialAd  = "AdMob_createInterstitialAd";
    private static final String k__destroyInterstitialAd = "AdMob_destroyInterstitialAd";
    private static final String k__hasRewardedVideo      = "AdMob_hasRewardedVideo";
    private static final String k__loadRewardedVideo     = "AdMob_loadRewardedVideo";
    private static final String k__showRewardedVideo     = "AdMob_showRewardedVideo";
    private static final String k__onRewarded            = "AdMob_onRewarded";
    private static final String k__onFailedToLoad        = "AdMob_onFailedToLoad";
    private static final String k__onLoaded              = "AdMob_onLoaded";
    private static final String k__onClosed              = "AdMob_onClosed";

    private static final Logger _logger = new Logger(AdMob.class.getName());

    private Activity                         _context;
    private RewardedVideoAd                  _rewardedVideoAd;
    private Map<String, AdMobBannerAd>       _bannerAds;
    private Map<String, AdMobInterstitialAd> _interstitialAds;

    public AdMob(Context context) {
        Utils.checkMainThread();
        _context = (Activity) context;

        _bannerAds = new HashMap<>();
        _interstitialAds = new HashMap<>();

        _rewardedVideoAd = MobileAds.getRewardedVideoAdInstance(_context);
        _rewardedVideoAd.setRewardedVideoAdListener(this);

        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "AdMob";
    }

    @Override
    public void onStart() {
    }

    @Override
    public void onStop() {
    }

    @Override
    public void onResume() {
        _rewardedVideoAd.resume(_context);
    }

    @Override
    public void onPause() {
        _rewardedVideoAd.pause(_context);
    }

    @Override
    public void onDestroy() {
        Utils.checkMainThread();
        deregisterHandlers();

        _rewardedVideoAd.destroy(_context);

        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).destroy();
        }
        _bannerAds.clear();
        _bannerAds = null;

        for (String key : _interstitialAds.keySet()) {
            _interstitialAds.get(key).destroy();
        }
        _interstitialAds.clear();
        _interstitialAds = null;

        _context = null;
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
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;

                String adId = (String) dict.get("adId");
                Integer adSizeIndex = (Integer) dict.get("adSize");
                AdSize adSize = AdMobBannerAd.adSizeFor(adSizeIndex);
                return Utils.toString(createBannerAd(adId, adSize));
            }
        }, k__createBannerAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                return Utils.toString(destroyBannerAd(adId));
            }
        }, k__destroyBannerAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(createInterstitialAd(placementId));
            }
        }, k__createInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(destroyInterstitialAd(placementId));
            }
        }, k__destroyInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                ;
                return Utils.toString(hasRewardedVideo());
            }
        }, k__destroyInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                loadRewardedVideo(adId);
                return "";
            }
        }, k__destroyInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                showRewardedVideo();
                return "";
            }
        }, k__destroyInterstitialAd);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__createBannerAd);
        bridge.deregisterHandler(k__destroyBannerAd);
        bridge.deregisterHandler(k__createInterstitialAd);
        bridge.deregisterHandler(k__destroyInterstitialAd);
        bridge.deregisterHandler(k__hasRewardedVideo);
        bridge.deregisterHandler(k__loadRewardedVideo);
        bridge.deregisterHandler(k__showRewardedVideo);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createBannerAd(@NonNull String adId, @NonNull AdSize size) {
        Utils.checkMainThread();
        if (_bannerAds.containsKey(adId)) {
            return false;
        }
        AdMobBannerAd ad = new AdMobBannerAd(_context, adId, size);
        _bannerAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyBannerAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_bannerAds.containsKey(adId)) {
            return false;
        }
        AdMobBannerAd ad = _bannerAds.get(adId);
        ad.destroy();
        _bannerAds.remove(adId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createInterstitialAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (_interstitialAds.containsKey(adId)) {
            return false;
        }
        AdMobInterstitialAd ad = new AdMobInterstitialAd(_context, adId);
        _interstitialAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyInterstitialAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_interstitialAds.containsKey(adId)) {
            return false;
        }
        AdMobInterstitialAd ad = _interstitialAds.get(adId);
        ad.destroy();
        _interstitialAds.remove(adId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedVideo() {
        Utils.checkMainThread();
        return _rewardedVideoAd.isLoaded();
    }

    @SuppressWarnings("WeakerAccess")
    public void loadRewardedVideo(@NonNull String adId) {
        Utils.checkMainThread();
        AdRequest request = new AdRequest.Builder().build();
        _rewardedVideoAd.loadAd(adId, request);
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedVideo() {
        Utils.checkMainThread();
        _rewardedVideoAd.show();
    }

    @Override
    public void onRewardedVideoAdLoaded() {
        _logger.info("onRewardedVideoAdLeftApplication");
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onLoaded);
    }

    @Override
    public void onRewardedVideoAdOpened() {
        _logger.info("onRewardedVideoAdLeftApplication");
        Utils.checkMainThread();
    }

    @Override
    public void onRewardedVideoStarted() {
        _logger.info("onRewardedVideoAdLeftApplication");
        Utils.checkMainThread();
    }

    @Override
    public void onRewardedVideoAdClosed() {
        _logger.info("onRewardedVideoAdClosed");
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onClosed);
    }

    @Override
    public void onRewarded(RewardItem rewardItem) {
        _logger.info("onRewarded");
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onRewarded);
    }

    @Override
    public void onRewardedVideoAdLeftApplication() {
        _logger.info("onRewardedVideoAdLeftApplication");
        Utils.checkMainThread();
    }

    @Override
    public void onRewardedVideoAdFailedToLoad(int errorCode) {
        _logger.info("onRewardedVideoAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();

        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad, String.valueOf(errorCode));
    }
}
