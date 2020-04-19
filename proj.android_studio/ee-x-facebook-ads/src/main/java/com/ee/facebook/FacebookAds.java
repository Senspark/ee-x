package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.ee.facebook.ads.BuildConfig;
import com.facebook.ads.AdSettings;
import com.facebook.ads.AdSize;
import com.facebook.ads.AudienceNetworkAds;

import java.util.HashMap;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class FacebookAds implements PluginProtocol {
    private static final String k__getTestDeviceHash = "FacebookAds_getTestDeviceHash";
    private static final String k__addTestDevice = "FacebookAds_addTestDevice";
    private static final String k__clearTestDevices = "FacebookAds_clearTestDevices";
    private static final String k__createBannerAd = "FacebookAds_createBannerAd";
    private static final String k__destroyBannerAd = "FacebookAds_destroyBannerAd";
    private static final String k__createNativeAd = "FacebookAds_createNativeAd";
    private static final String k__destroyNativeAd = "FacebookAds_destroyNativeAd";
    private static final String k__createInterstitialAd = "FacebookAds_createInterstitialAd";
    private static final String k__destroyInterstitialAd = "FacebookAds_destroyInterstitialAd";
    private static final String k__createRewardVideoAd = "FacebookAds_createRewardVideoAd";
    private static final String k__destroyRewardVideoAd = "FacebookAds_destroyRewardVideoAd";

    private static final String k__ad_id = "ad_id";
    private static final String k__ad_size = "ad_size";
    private static final String k__layout_name = "layout_name";
    private static final String k__identifiers = "identifiers";

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private Map<String, FacebookBannerAd> _bannerAds;
    private Map<String, FacebookNativeAd> _nativeAds;
    private Map<String, FacebookInterstitialAd> _interstitialAds;
    private Map<String, FacebookRewardedAd> _rewardVideoAds;

    public FacebookAds(Context context) {
        Utils.checkMainThread();
        _context = context;
        _activity = null;
        _bridge = MessageBridge.getInstance();

        _bannerAds = new HashMap<>();
        _nativeAds = new HashMap<>();
        _interstitialAds = new HashMap<>();
        _rewardVideoAds = new HashMap<>();

        if (!AudienceNetworkAds.isInitialized(context)) {
            if (BuildConfig.DEBUG) {
                AdSettings.setDebugBuild(true);
            }

            AudienceNetworkAds.initialize(context);
            AudienceNetworkAds.isInAdsProcess(context);
        }

        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "FacebookAds";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).onCreate(activity);
        }
        for (String key : _nativeAds.keySet()) {
            _nativeAds.get(key).onCreate(activity);
        }
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
        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).onDestroy(_activity);
        }
        for (String key : _nativeAds.keySet()) {
            _nativeAds.get(key).onDestroy(_activity);
        }
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
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

        for (String key : _rewardVideoAds.keySet()) {
            _rewardVideoAds.get(key).destroy();
        }
        _rewardVideoAds.clear();
        _rewardVideoAds = null;

        _context = null;
        _bridge = null;
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
        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getTestDeviceHash();
            }
        }, k__getTestDeviceHash);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String hash = message;
                addTestDevice(hash);
                return "";
            }
        }, k__addTestDevice);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                clearTestDevices();
                return "";
            }
        }, k__clearTestDevices);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assertThat(dict).isNotNull();

                String adId = (String) dict.get(k__ad_id);
                Integer adSizeIndex = (Integer) dict.get(k__ad_size);
                AdSize adSize = FacebookBannerAd.adSizeFor(adSizeIndex);
                return Utils.toString(createBannerAd(adId, adSize));
            }
        }, k__createBannerAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                return Utils.toString(destroyBannerAd(adId));
            }
        }, k__destroyBannerAd);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assertThat(dict).isNotNull();

                String adId = (String) dict.get(k__ad_id);
                String layoutName = (String) dict.get(k__layout_name);
                @SuppressWarnings("unchecked") Map<String, Object> identifiers_raw =
                    (Map<String, Object>) dict.get(k__identifiers);
                Map<String, String> identifiers = new HashMap<>();
                for (String key : identifiers_raw.keySet()) {
                    identifiers.put(key, (String) identifiers_raw.get(key));
                }
                return Utils.toString(createNativeAd(adId, layoutName, identifiers));
            }
        }, k__createNativeAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String adId = message;
                return Utils.toString(destroyNativeAd(adId));
            }
        }, k__destroyNativeAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(createInterstitialAd(placementId));
            }
        }, k__createInterstitialAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(destroyInterstitialAd(placementId));
            }
        }, k__destroyInterstitialAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(createRewardVideoAd(placementId));
            }
        }, k__createRewardVideoAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(destroyRewardVideoAd(placementId));
            }
        }, k__destroyRewardVideoAd);

    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__getTestDeviceHash);
        _bridge.deregisterHandler(k__addTestDevice);
        _bridge.deregisterHandler(k__clearTestDevices);
        _bridge.deregisterHandler(k__createBannerAd);
        _bridge.deregisterHandler(k__destroyBannerAd);
        _bridge.deregisterHandler(k__createNativeAd);
        _bridge.deregisterHandler(k__destroyNativeAd);
        _bridge.deregisterHandler(k__createInterstitialAd);
        _bridge.deregisterHandler(k__destroyInterstitialAd);
    }

    @SuppressWarnings("WeakerAccess")
    @NonNull
    public String getTestDeviceHash() {
        return "";
    }

    @SuppressWarnings("WeakerAccess")
    public void addTestDevice(@NonNull String hash) {
        Utils.checkMainThread();
        AdSettings.addTestDevice(hash);
    }

    @SuppressWarnings("WeakerAccess")
    public void clearTestDevices() {
        Utils.checkMainThread();
        AdSettings.clearTestDevices();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createBannerAd(@NonNull String adId, @NonNull AdSize adSize) {
        Utils.checkMainThread();
        if (_bannerAds.containsKey(adId)) {
            return false;
        }
        FacebookBannerAd ad = new FacebookBannerAd(_context, _activity, adId, adSize);
        _bannerAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyBannerAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_bannerAds.containsKey(adId)) {
            return false;
        }
        FacebookBannerAd ad = _bannerAds.get(adId);
        ad.destroy();
        _bannerAds.remove(adId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createNativeAd(@NonNull String adId, @NonNull String layoutName,
                                  @NonNull Map<String, String> identifiers) {
        Utils.checkMainThread();
        if (_nativeAds.containsKey(adId)) {
            return false;
        }
        FacebookNativeAd ad =
            new FacebookNativeAd(_context, _activity, adId, layoutName, identifiers);
        _nativeAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyNativeAd(@NonNull String adId) {
        Utils.checkMainThread();
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
        Utils.checkMainThread();
        if (_interstitialAds.containsKey(placementId)) {
            return false;
        }
        FacebookInterstitialAd ad = new FacebookInterstitialAd(_context, placementId);
        _interstitialAds.put(placementId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyInterstitialAd(@NonNull String placementId) {
        Utils.checkMainThread();
        if (!_interstitialAds.containsKey(placementId)) {
            return false;
        }
        FacebookInterstitialAd ad = _interstitialAds.get(placementId);
        ad.destroy();
        _interstitialAds.remove(placementId);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    private boolean createRewardVideoAd(@NonNull String placementId) {
        Utils.checkMainThread();
        if (_rewardVideoAds.containsKey(placementId)) {
            return false;
        }
        FacebookRewardedAd ad = new FacebookRewardedAd(_context, placementId);
        _rewardVideoAds.put(placementId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyRewardVideoAd(@NonNull String placementId) {
        Utils.checkMainThread();
        if (!_rewardVideoAds.containsKey(placementId)) {
            return false;
        }
        FacebookRewardedAd ad = _rewardVideoAds.get(placementId);
        ad.destroy();
        _rewardVideoAds.remove(placementId);
        return true;
    }
}
