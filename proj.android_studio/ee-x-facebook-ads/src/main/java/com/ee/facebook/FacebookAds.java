package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import com.ee.core.Logger;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdSettings;
import com.facebook.ads.AdSize;
import com.facebook.ads.InterstitialAd;
import com.facebook.ads.InterstitialAdListener;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class FacebookAds implements PluginProtocol, InterstitialAdListener {
    private static final String k__createBannerAd      = "FacebookAds_createBannerAd";
    private static final String k__destroyBannerAd     = "FacebookAds_destroyBannerAd";
    private static final String k__createNativeAd      = "FacebookAds_createNativeAd";
    private static final String k__destroyNativeAd     = "FacebookAds_destroyNativeAd";
    private static final String k__cacheInterstitialAd = "FacebookAds_cacheInterstitialAd";
    private static final String k__showInterstitialAd  = "FacebookAds_showInterstitialAd";
    private static final String k__cppCallback         = "FacebookAds_cppCallback";

    private static final Logger _logger = new Logger(FacebookAds.class.getName());

    private Activity                      _context;
    private InterstitialAd                _interstitialAd;
    private Map<String, FacebookBannerAd> _bannerAds;
    private Map<String, FacebookNativeAd> _nativeAds;

    public FacebookAds(Context context) {
        _context = (Activity) context;

        _interstitialAd = null;
        _bannerAds = new HashMap<>();
        _nativeAds = new HashMap<>();

        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FacebookAds";
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
        if (_interstitialAd != null) {
            _interstitialAd.destroy();
            _interstitialAd = null;
        }
        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).destroy();
        }
        for (String key : _nativeAds.keySet()) {
            _nativeAds.get(key).destroy();
        }
        _bannerAds.clear();
        _bannerAds = null;
        _nativeAds.clear();
        _nativeAds = null;
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
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;

                String adId = (String) dict.get("adId");
                Integer adSizeIndex = (Integer) dict.get("adSize");
                AdSize adSize = FacebookBannerAd.adSizeFor(adSizeIndex);
                return createBannerAd(adId, adSize) ? "true" : "false";
            }
        }, k__createBannerAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                return destroyBannerAd(adId) ? "true" : "false";
            }
        }, k__destroyBannerAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assert dict != null;

                FacebookNativeAdBuilder builder = new FacebookNativeAdBuilder()
                    .setAdId((String) dict.get("adId"))
                    .setLayoutName((String) dict.get("layoutName"))
                    .setIcon((String) dict.get("icon"))
                    .setTitle((String) dict.get("title"))
                    .setMedia((String) dict.get("media"))
                    .setSocialContext((String) dict.get("socialContext"))
                    .setAdChoices((String) dict.get("adChoices"))
                    .setBody((String) dict.get("body"))
                    .setAction((String) dict.get("action"));
                return createNativeAd(builder) ? "true" : "false";
            }
        }, k__createNativeAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                return destroyNativeAd(adId) ? "true" : "false";
            }
        }, k__destroyNativeAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                cacheInterstitialAd(adId);
                return "";
            }
        }, k__cacheInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return showInterstitialAd() ? "true" : "false";
            }
        }, k__showInterstitialAd);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__createBannerAd);
        bridge.deregisterHandler(k__destroyBannerAd);
        bridge.deregisterHandler(k__createNativeAd);
        bridge.deregisterHandler(k__destroyNativeAd);
        bridge.deregisterHandler(k__cacheInterstitialAd);
        bridge.deregisterHandler(k__showInterstitialAd);
    }

    public void initTestDevice() {
        // test device
        //find device hash in log debug AdSetting tag
        //AdSettings: Test mode device hash: 75b86cd356c31245209539b475ea5630
        //        AdSettings.addTestDevice("ea9b72adbe82f18603e900ae0a5d7618");//Bphone
        //        AdSettings.addTestDevice("75b86cd356c31245209539b475ea5630");//nexus5

        AdSettings.clearTestDevices();
    }

    public boolean createBannerAd(@NonNull String adId, @NonNull AdSize adSize) {
        if (_bannerAds.containsKey(adId)) {
            return false;
        }
        FacebookBannerAd ad = new FacebookBannerAd(_context, adId, adSize);
        _bannerAds.put(adId, ad);
        return true;
    }

    public boolean destroyBannerAd(@NonNull String adId) {
        if (!_bannerAds.containsKey(adId)) {
            return false;
        }
        FacebookBannerAd ad = _bannerAds.get(adId);
        ad.destroy();
        _bannerAds.remove(adId);
        return true;
    }

    public boolean createNativeAd(@NonNull FacebookNativeAdBuilder builder) {
        if (_nativeAds.containsKey(builder.adId)) {
            return false;
        }
        FacebookNativeAd ad = new FacebookNativeAd(_context, builder);
        _nativeAds.put(builder.adId, ad);
        return true;
    }

    public boolean destroyNativeAd(@NonNull String adId) {
        if (!_nativeAds.containsKey(adId)) {
            return false;
        }
        FacebookNativeAd ad = _nativeAds.get(adId);
        ad.destroy();
        _nativeAds.remove(adId);
        return true;
    }

    private InterstitialAd createInterstitialAd(@NonNull String adId) {
        InterstitialAd ad = new InterstitialAd(_context, adId);
        ad.setAdListener(this);
        ad.loadAd();
        return ad;
    }


    @SuppressWarnings("WeakerAccess")
    public void cacheInterstitialAd(@NonNull final String adId) {
        initTestDevice();
        _context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (_interstitialAd != null) {
                    _interstitialAd = createInterstitialAd(adId);
                    return;
                }
                // noinspection ConstantConditions
                if (_interstitialAd.getPlacementId().equals(adId)) {
                    _interstitialAd.loadAd();
                } else {
                    _interstitialAd = createInterstitialAd(adId);
                }
            }
        });
    }

    @SuppressWarnings({"WeakerAccess", "SimplifiableIfStatement"})
    public boolean hasInterstitialAd() {
        if (_interstitialAd == null) {
            return false;
        }
        return _interstitialAd.isAdLoaded();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean showInterstitialAd() {
        if (!hasInterstitialAd()) {
            return false;
        }
        _context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                _interstitialAd.show();
            }
        });
        return true;
    }

    @Override
    public void onInterstitialDisplayed(Ad ad) {
        _logger.info("onInterstitialDisplayed");
    }

    @Override
    public void onInterstitialDismissed(Ad ad) {
        _logger.info("onInterstitialDismissed");
        cacheInterstitialAd(ad.getPlacementId());
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdCLicked");
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
    }
}
