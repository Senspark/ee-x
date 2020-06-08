package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Point;

import androidx.annotation.NonNull;

import com.ee.core.IMessageBridge;
import com.ee.core.IPlugin;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.RequestConfiguration;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/13/17.
 */

public class AdMob implements IPlugin {
    private static final String kPrefix = "AdMob";

    private static final String k__initialize = kPrefix + "_initialize";
    private static final String k__getEmulatorTestDeviceHash = kPrefix + "_getEmulatorTestDeviceHash";
    private static final String k__addTestDevice = kPrefix + "_addTestDevice";

    private static final String k__getBannerAdSize = kPrefix + "_getBannerAdSize";
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
    private AdMobBannerHelper _bannerHelper;
    private List<String> _testDevices;
    private Map<String, AdMobBannerAd> _bannerAds;
    private Map<String, AdMobNativeAd> _nativeAds;
    private Map<String, AdMobInterstitialAd> _interstitialAds;
    private Map<String, AdMobRewardedAd> _rewardedAds;

    public AdMob(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Utils.checkMainThread();
        _context = context;
        _bridge = bridge;
        _bannerHelper = new AdMobBannerHelper(context);
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
        for (String key : _rewardedAds.keySet()) {
            _rewardedAds.get(key).onCreate(activity);
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
            for (String key : _rewardedAds.keySet()) {
                _rewardedAds.get(key).onDestroy(_activity);
            }
            _activity = null;
        }
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();

        _context = null;
        _bridge = null;

        _bannerHelper = null;
        _testDevices = null;

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
    }

    @Override
    public boolean onActivityResult(int requestCode, int responseCode, Intent data) {
        return false;
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__initialize, message -> {
            initialize();
            return "";
        });

        _bridge.registerHandler(k__getEmulatorTestDeviceHash, message ->
            getEmulatorTestDeviceHash());

        _bridge.registerHandler(k__addTestDevice, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String hash = message;
            addTestDevice(hash);
            return "";
        });

        _bridge.registerHandler(k__getBannerAdSize, message -> {
            int sizeId = Integer.parseInt(message);
            Point size = getBannerAdSize(sizeId);
            Map<String, Object> dict = new HashMap<>();
            dict.put("width", size.x);
            dict.put("height", size.y);
            return Objects.requireNonNull(JsonUtils.convertDictionaryToString(dict));
        });

        _bridge.registerHandler(k__createBannerAd, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String adId = (String) dict.get(k__ad_id);
            Integer sizeId = (Integer) dict.get(k__ad_size);
            assertThat(adId).isNotNull();
            assertThat(sizeId).isNotNull();

            AdSize adSize = _bannerHelper.getAdSize(sizeId);
            return Utils.toString(createBannerAd(adId, adSize));
        });

        _bridge.registerHandler(k__destroyBannerAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(destroyBannerAd(adId));
        });

        _bridge.registerHandler(k__createNativeAd, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String adId = (String) dict.get(k__ad_id);
            String layoutName = (String) dict.get(k__layout_name);
            assertThat(adId).isNotNull();
            assertThat(layoutName).isNotNull();

            @SuppressWarnings("unchecked")
            Map<String, Object> identifiers_raw = (Map<String, Object>) dict.get(k__identifiers);
            assertThat(identifiers_raw).isNotNull();

            Map<String, String> identifiers = new HashMap<>();
            for (String key : identifiers_raw.keySet()) {
                identifiers.put(key, (String) identifiers_raw.get(key));
            }
            return Utils.toString(createNativeAd(adId, layoutName, identifiers));
        });

        _bridge.registerHandler(k__destroyNativeAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(destroyNativeAd(adId));
        });

        _bridge.registerHandler(k__createInterstitialAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(createInterstitialAd(adId));
        });

        _bridge.registerHandler(k__destroyInterstitialAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(destroyInterstitialAd(adId));
        });

        _bridge.registerHandler(k__createRewardedAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(createRewardedAd(adId));
        });

        _bridge.registerHandler(k__destroyRewardedAd, message -> {
            @SuppressWarnings("UnnecessaryLocalVariable")
            String adId = message;
            return Utils.toString(destroyRewardedAd(adId));
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__initialize);
        _bridge.deregisterHandler(k__getEmulatorTestDeviceHash);
        _bridge.deregisterHandler(k__addTestDevice);
        _bridge.deregisterHandler(k__getBannerAdSize);
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
    public void initialize() {
        Utils.checkMainThread();
        MobileAds.initialize(_context, initializationStatus -> {
            // onInitializationComplete
        });
    }

    @NonNull
    public String getEmulatorTestDeviceHash() {
        return AdRequest.DEVICE_ID_EMULATOR;
    }

    public void addTestDevice(@NonNull String hash) {
        Utils.checkMainThread();
        _testDevices.add(hash);
        RequestConfiguration configuration = new RequestConfiguration.Builder()
            .setTestDeviceIds(_testDevices)
            .build();
        MobileAds.setRequestConfiguration(configuration);
    }

    @NonNull
    public Point getBannerAdSize(int sizeId) {
        return _bannerHelper.getSize(sizeId);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createBannerAd(@NonNull String adId, @NonNull AdSize size) {
        Utils.checkMainThread();
        if (_bannerAds.containsKey(adId)) {
            return false;
        }
        AdMobBannerAd ad = new AdMobBannerAd(_context, _activity, _bridge, adId, size);
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

    public boolean createNativeAd(@NonNull String adId, @NonNull String layoutName,
                                  @NonNull Map<String, String> identifiers) {
        Utils.checkMainThread();
        if (_nativeAds.containsKey(adId)) {
            return false;
        }
        AdMobNativeAd ad =
            new AdMobNativeAd(_context, _activity, _bridge, adId, layoutName, identifiers);
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
        AdMobInterstitialAd ad = new AdMobInterstitialAd(_context, _bridge, adId);
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
        AdMobRewardedAd ad = new AdMobRewardedAd(_context, _activity, _bridge, adId);
        _rewardedAds.put(adId, ad);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyRewardedAd(@NonNull String adId) {
        Utils.checkMainThread();
        if (!_rewardedAds.containsKey(adId)) {
            return false;
        }
        AdMobRewardedAd ad = _rewardedAds.get(adId);
        ad.destroy();
        _rewardedAds.remove(adId);
        return true;
    }
}
