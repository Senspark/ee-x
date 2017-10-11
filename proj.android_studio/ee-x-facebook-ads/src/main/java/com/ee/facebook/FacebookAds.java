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
import com.facebook.ads.AdSettings;
import com.facebook.ads.AdSize;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class FacebookAds implements PluginProtocol {
    private static final String k__getTestDeviceHash     = "FacebookAds_getTestDeviceHash";
    private static final String k__addTestDevice         = "FacebookAds_addTestDevice";
    private static final String k__clearTestDevices      = "FacebookAds_clearTestDevices";
    private static final String k__createBannerAd        = "FacebookAds_createBannerAd";
    private static final String k__destroyBannerAd       = "FacebookAds_destroyBannerAd";
    private static final String k__createNativeAd        = "FacebookAds_createNativeAd";
    private static final String k__destroyNativeAd       = "FacebookAds_destroyNativeAd";
    private static final String k__createInterstitialAd  = "FacebookAds_createInterstitialAd";
    private static final String k__destroyInterstitialAd = "FacebookAds_destroyInterstitialAd";

    private static final Logger _logger = new Logger(FacebookAds.class.getName());

    private Activity                            _context;
    private Map<String, FacebookBannerAd>       _bannerAds;
    private Map<String, FacebookNativeAd>       _nativeAds;
    private Map<String, FacebookInterstitialAd> _interstitialAds;

    public FacebookAds(Context context) {
        _context = (Activity) context;

        _bannerAds = new HashMap<>();
        _nativeAds = new HashMap<>();
        _interstitialAds = new HashMap<>();

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

        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).destroy();
        }
        _bannerAds.clear();
        _bannerAds = null;

        for (String key : _nativeAds.keySet()) {
            _nativeAds.get(key).destroy();
        }
        _nativeAds.clear();
        _nativeAds = null;

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
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getTestDeviceHash();
            }
        }, k__getTestDeviceHash);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String hash = message;
                addTestDevice(hash);
                return "";
            }
        }, k__addTestDevice);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                clearTestDevices();
                return "";
            }
        }, k__clearTestDevices);

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
                String placementId = message;
                return createInterstitialAd(placementId) ? "true" : "false";
            }
        }, k__createInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return destroyInterstitialAd(placementId) ? "true" : "false";
            }
        }, k__destroyInterstitialAd);
    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__getTestDeviceHash);
        bridge.deregisterHandler(k__addTestDevice);
        bridge.deregisterHandler(k__clearTestDevices);
        bridge.deregisterHandler(k__createBannerAd);
        bridge.deregisterHandler(k__destroyBannerAd);
        bridge.deregisterHandler(k__createNativeAd);
        bridge.deregisterHandler(k__destroyNativeAd);
        bridge.deregisterHandler(k__createInterstitialAd);
        bridge.deregisterHandler(k__destroyInterstitialAd);
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public String getTestDeviceHash() {
        return "";
    }

    @SuppressWarnings("WeakerAccess")
    public void addTestDevice(@NonNull String hash) {
        AdSettings.addTestDevice(hash);
    }

    @SuppressWarnings("WeakerAccess")
    public void clearTestDevices() {
        AdSettings.clearTestDevices();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createBannerAd(@NonNull String adId, @NonNull AdSize adSize) {
        if (_bannerAds.containsKey(adId)) {
            return false;
        }
        FacebookBannerAd ad = new FacebookBannerAd(_context, adId, adSize);
        _bannerAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyBannerAd(@NonNull String adId) {
        if (!_bannerAds.containsKey(adId)) {
            return false;
        }
        FacebookBannerAd ad = _bannerAds.get(adId);
        ad.destroy();
        _bannerAds.remove(adId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createNativeAd(@NonNull FacebookNativeAdBuilder builder) {
        if (_nativeAds.containsKey(builder.adId)) {
            return false;
        }
        FacebookNativeAd ad = new FacebookNativeAd(_context, builder);
        _nativeAds.put(builder.adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyNativeAd(@NonNull String adId) {
        if (!_nativeAds.containsKey(adId)) {
            return false;
        }
        FacebookNativeAd ad = _nativeAds.get(adId);
        ad.destroy();
        _nativeAds.remove(adId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    private boolean createInterstitialAd(@NonNull String placementId) {
        if (_interstitialAds.containsKey(placementId)) {
            return false;
        }
        FacebookInterstitialAd ad = new FacebookInterstitialAd(_context, placementId);
        _interstitialAds.put(placementId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyInterstitialAd(@NonNull String placementId) {
        if (!_interstitialAds.containsKey(placementId)) {
            return false;
        }
        FacebookInterstitialAd ad = _interstitialAds.get(placementId);
        ad.destroy();
        _interstitialAds.remove(placementId);
        return true;
    }
}
