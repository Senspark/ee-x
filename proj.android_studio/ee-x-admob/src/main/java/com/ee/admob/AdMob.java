package com.ee.admob;

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
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.RequestConfiguration;
import com.google.android.gms.ads.initialization.InitializationStatus;
import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/13/17.
 */

public class AdMob implements PluginProtocol {
    private static final String kPrefix = "AdMob";

    private static final String k__initialize = kPrefix + "_initialize";
    private static final String k__getEmulatorTestDeviceHash = kPrefix + "_getEmulatorTestDeviceHash";
    private static final String k__addTestDevice = kPrefix + "_addTestDevice";

    private static final String k__createBannerAd = kPrefix + "_createBannerAd";
    private static final String k__destroyBannerAd = kPrefix + "_destroyBannerAd";

    private static final String k__createNativeAd = kPrefix + "_createNativeAd";
    private static final String k__destroyNativeAd = kPrefix + "_destroyNativeAd";

    private static final String k__createInterstitialAd = kPrefix + "_createInterstitialAd";
    private static final String k__destroyInterstitialAd = kPrefix + "_destroyInterstitialAd";

    private static final String k__createRewardedAd = kPrefix + "_createRewardedAd";
    private static final String k__destroyRewardedAd = kPrefix + "_destroyRewardedAd";

    private static final String k__ad_id = "ad_id";
    private static final String k__ad_size = "ad_size";
    private static final String k__layout_name = "layout_name";
    private static final String k__identifiers = "identifiers";

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private List<String> _testDevices;
    private Map<String, AdMobBannerAd> _bannerAds;
    private Map<String, AdMobNativeAd> _nativeAds;
    private Map<String, AdMobInterstitialAd> _interstitialAds;
    private Map<String, AdMobRewardVideoAd> _rewardedAds;

    public AdMob(Context context) {
        Utils.checkMainThread();
        _context = context;
        _activity = null;
        _bridge = MessageBridge.getInstance();
        _testDevices = new ArrayList<>();
        _bannerAds = new HashMap<>();
        _nativeAds = new HashMap<>();
        _interstitialAds = new HashMap<>();
        _rewardedAds = new HashMap<>();
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "AdMob";
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
        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).onResume();
        }
    }

    @Override
    public void onPause() {
        for (String key : _bannerAds.keySet()) {
            _bannerAds.get(key).onPause();
        }
    }

    @Override
    public void onDestroy() {
        if (_activity != null) {
            for (String key : _bannerAds.keySet()) {
                _bannerAds.get(key).onDestroy(_activity);
            }
            for (String key : _nativeAds.keySet()) {
                _nativeAds.get(key).onDestroy(_activity);
            }
            _activity = null;
        }
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

        _rewardedAds.clear();
        _rewardedAds = null;

        _bridge = null;
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
        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String applicationId = message;
                initialize(applicationId);
                return "";
            }
        }, k__initialize);

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return getEmulatorTestDeviceHash();
            }
        }, k__getEmulatorTestDeviceHash);

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
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
                assertThat(dict).isNotNull();

                String adId = (String) dict.get(k__ad_id);
                Integer adSizeIndex = (Integer) dict.get(k__ad_size);
                AdSize adSize = AdMobBannerAd.adSizeFor(adSizeIndex);
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
                @SuppressWarnings("unchecked") Map<String, Object> identifiers_raw = (Map<String, Object>) dict.get(k__identifiers);
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
                return Utils.toString(createRewardedAd(placementId));
            }
        }, k__createRewardedAd);

        _bridge.registerHandler(new MessageHandler() {
            @SuppressWarnings("UnnecessaryLocalVariable")
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                String placementId = message;
                return Utils.toString(destroyRewardedAd(placementId));
            }
        }, k__destroyRewardedAd);
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
        _bridge.deregisterHandler(k__getEmulatorTestDeviceHash);
        _bridge.deregisterHandler(k__addTestDevice);
        _bridge.deregisterHandler(k__createBannerAd);
        _bridge.deregisterHandler(k__destroyBannerAd);
        _bridge.deregisterHandler(k__createNativeAd);
        _bridge.deregisterHandler(k__destroyNativeAd);
        _bridge.deregisterHandler(k__createInterstitialAd);
        _bridge.deregisterHandler(k__destroyInterstitialAd);
        _bridge.deregisterHandler(k__createRewardedAd);
        _bridge.deregisterHandler(k__destroyRewardedAd);
    }

    @SuppressWarnings("WeakerAccess")
    public void initialize(@NonNull String applicationId) {
        Utils.checkMainThread();
        MobileAds.initialize(_context, new OnInitializationCompleteListener() {
            @Override
            public void onInitializationComplete(InitializationStatus initializationStatus) {
                // onInitializationComplete
            }
        });
    }

    @NonNull
    public String getEmulatorTestDeviceHash() {
        return AdRequest.DEVICE_ID_EMULATOR;
    }

    public void addTestDevice(@NonNull String hash) {
        Utils.checkMainThread();
        _testDevices.add(hash);
        RequestConfiguration configuration = new RequestConfiguration.Builder().setTestDeviceIds(_testDevices).build();
        MobileAds.setRequestConfiguration(configuration);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createBannerAd(@NonNull String adId, @NonNull AdSize size) {
        Utils.checkMainThread();
        if (_bannerAds.containsKey(adId)) {
            return false;
        }
        AdMobBannerAd ad = new AdMobBannerAd(_context, _activity, adId, size);
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

    public boolean createNativeAd(@NonNull String adId, @NonNull String layoutName, @NonNull Map<String, String> identifiers) {
        Utils.checkMainThread();
        if (_nativeAds.containsKey(adId)) {
            return false;
        }
        AdMobNativeAd ad = new AdMobNativeAd(_context, _activity, adId, layoutName, identifiers);
        _nativeAds.put(adId, ad);
        return true;
    }

    public boolean destroyNativeAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_nativeAds.containsKey(adId)) {
            return false;
        }
        AdMobNativeAd ad = _nativeAds.get(adId);
        ad.destroy();
        _nativeAds.remove(adId);
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
    public boolean createRewardedAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (_rewardedAds.containsKey(adId)) {
            return false;
        }
        AdMobRewardVideoAd ad = new AdMobRewardVideoAd(_activity, _context, adId);
        _rewardedAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyRewardedAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_rewardedAds.containsKey(adId)) {
            return false;
        }
        AdMobRewardVideoAd ad = _rewardedAds.get(adId);
        ad.destroy();
        _rewardedAds.remove(adId);
        return true;
    }
}
