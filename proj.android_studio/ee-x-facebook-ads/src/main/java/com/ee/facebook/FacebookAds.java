package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;

import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.ee.core.PluginProtocol;
import com.ee.core.internal.DictionaryUtils;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.MessageHandler;


import java.util.HashMap;
import java.util.Map;

import com.facebook.ads.*;
import com.facebook.ads.NativeAd;

/**
 * Created by Pham Xuan Han on 17/05/17.
 */
public class FacebookAds implements PluginProtocol {

    private static final String k__facebookads_initFBAdsInterstitial = "__facebookads_initFBAdsInterstitial";
    private static final String k__facebookads_initFBAdsNativeAds = "__facebookads_initFBAdsNativeAds";
    private static final String k__facebookads_initFBAdsBanner = "__facebookads_initFBAdsBanner";

    private static final String k__facebookads_showBannerAd = "__facebookads_showBannerAd";
    private static final String k__facebookads_showInterstitialAd = "__facebookads_showInterstitialAd";
    private static final String k__facebookads_showNativeAd = "__facebookads_showNativeAd";
    private static final String k__facebookads_showRewardedAd = "__facebookads_showRewardedAd";

    private static final String k__facebookads_cacheRewardedAd = "__facebookads_cacheRewardedAd";
    private static final String k__facebookads_cacheInterstitialAd = "__facebookads_cacheInterstitialAd";

    private static final String k__facebookads_hideNativeAd = "__facebookads_hideNativeAd";
    private static final String k__facebookads_hideBannerAd = "__facebookads_hideBannerAd";

    private static final String k__facebookads_hasInterstitialAd = "__facebookads_hasInterstitialAd";
    private static final String k__facebookads_hasRewardedAd = "__facebookads_hasRewardedAd";
    private static final String k__facebookads_hasNativeAd = "__facebookads_hasNativeAd";


    private static final String k__facebookads_callback = "__facebookads_callback";

    private InterstitialAd _interstitialAd;
    private String _interstitialID;
    private boolean _canShowInterstitialAd;

    private Activity _context;

    private Map<String, NativeAd> _dictFBNativeAd;
    private Map<String, View> _dictFBNativeAdView;
    private Map<String, String> _dictFBNativeAdReady;

    public FacebookAds(Context context) {
        _context = (Activity) context;
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

                String InterstitialID = (String) dict.get("InterstitialID");

                initFBAdsInterstitial(InterstitialID);
                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_initFBAdsInterstitial);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String BannerID = (String) dict.get("BannerID");

                initFBAdsBanner(BannerID);

                return DictionaryUtils.emptyResult();

            }
        }, k__facebookads_initFBAdsBanner);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String NativeID = (String) dict.get("NativeID");
                String layout = (String) dict.get("layout");
                initFBAdsNativeAds(NativeID, layout);

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_initFBAdsNativeAds);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String adsID = (String) dict.get("adsID");
                cacheRewardedAd(adsID);

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_cacheRewardedAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String adsID = (String) dict.get("adsID");
                cacheInterstitialAd(adsID);

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_cacheInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                showInterstitialAd();

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_showInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                showRewardedAd();

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_showRewardedAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {

                hideBannerAd();

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_hideBannerAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String adsID = (String) dict.get("adsID");
                int pos = (int) dict.get("pos");
                showBannerAd(adsID, pos);

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_showBannerAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String adsID = (String) dict.get("adsID");
                hideNativeAd(adsID);

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_hideNativeAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String adsID = (String) dict.get("adsID");
                int width = (int) dict.get("width");
                int height = (int) dict.get("height");
                int x = (int) dict.get("x");
                int y = (int) dict.get("y");
                showNativeAd(adsID, width, height, x, y);

                return DictionaryUtils.emptyResult();
            }
        }, k__facebookads_showNativeAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                return (hasInterstitialAd()) ? "true" : "false";
            }
        }, k__facebookads_hasInterstitialAd);

        bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String msg) {
                Map<String, Object> dict = JsonUtils.convertStringToDictionary(msg);
                assert dict != null;

                String adsID = (String) dict.get("adsID");
                return (hasNativeAd(adsID)) ? "true" : "false";
            }
        }, k__facebookads_hasNativeAd);


    }

    private void deregisterHandlers() {
        MessageBridge bridge = MessageBridge.getInstance();

        bridge.deregisterHandler(k__facebookads_initFBAdsBanner);
        bridge.deregisterHandler(k__facebookads_initFBAdsInterstitial);
        bridge.deregisterHandler(k__facebookads_initFBAdsNativeAds);

        bridge.deregisterHandler(k__facebookads_showNativeAd);
        bridge.deregisterHandler(k__facebookads_showInterstitialAd);
        bridge.deregisterHandler(k__facebookads_showBannerAd);
        bridge.deregisterHandler(k__facebookads_showRewardedAd);

        bridge.deregisterHandler(k__facebookads_hideNativeAd);
        bridge.deregisterHandler(k__facebookads_hideBannerAd);

        //bridge.deregisterHandler(k__facebookads_hasRewardedAd);
        bridge.deregisterHandler(k__facebookads_hasInterstitialAd);

        bridge.deregisterHandler(k__facebookads_cacheRewardedAd);
        bridge.deregisterHandler(k__facebookads_cacheInterstitialAd);
    }

    public void initTestDevice() {
        // test device
        //find device hash in log debug AdSetting tag
        //AdSettings: Test mode device hash: 75b86cd356c31245209539b475ea5630
//        AdSettings.addTestDevice("ea9b72adbe82f18603e900ae0a5d7618");//Bphone
//        AdSettings.addTestDevice("75b86cd356c31245209539b475ea5630");//nexus5

        AdSettings.clearTestDevices();
    }

    @SuppressWarnings("WeakerAccess")
    public void initFBAdsInterstitial(final @NonNull String InterstitialID) {
        this.initTestDevice();

        _context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                _interstitialID = InterstitialID;
                _canShowInterstitialAd = false;



                // inter ad
                _interstitialAd = new InterstitialAd(_context, InterstitialID);
                _interstitialAd.setAdListener(new InterstitialAdListener() {
                    @Override
                    public void onInterstitialDisplayed(Ad ad) {
                        // Interstitial displayed callback
                    }

                    @Override
                    public void onInterstitialDismissed(Ad ad) {
                        // Interstitial dismissed callback
                        Log.d("FBAds", "interstitialAdDidClose");
                        MessageBridge.getInstance().callCpp(k__facebookads_callback, "interstitialAdDidClose");
                    }

                    @Override
                    public void onError(Ad ad, AdError adError) {
                        // Ad error callback
                        Log.d("FBAds", "interstitialAd didFailWithError  " + adError.getErrorMessage());
                        MessageBridge.getInstance().callCpp(k__facebookads_callback, "interstitialAd didFailWithError");
                    }

                    @Override
                    public void onAdLoaded(Ad ad) {
                        // Show the ad when it's done loading.
                        Log.d("FBAds", "interstitialAdDidLoad");
                        _canShowInterstitialAd = true;
                        MessageBridge.getInstance().callCpp(k__facebookads_callback, "interstitialAdDidLoad");
                    }

                    @Override
                    public void onAdClicked(Ad ad) {
                        // Ad clicked callback
                    }

                    @Override
                    public void onLoggingImpression(Ad ad) {
                        // Ad impression logged callback
                    }
                });

                Log.d("FBAds", "Init Interstitial");


                _interstitialAd.loadAd();
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public void initFBAdsNativeAds(final @NonNull String NativeID, final @NonNull String layoutName) {

        _context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                initTestDevice();

                if (_dictFBNativeAd == null) {
                    _dictFBNativeAd = new HashMap<String, NativeAd>();
                    _dictFBNativeAdView = new HashMap<String, View>();
                    _dictFBNativeAdReady = new HashMap<String, String>();
                }

                // Add the Ad view into the ad container.
                FrameLayout layout = (FrameLayout)_context.findViewById(android.R.id.content).getRootView();

                int layoutID = _context.getResources().getIdentifier(layoutName, "layout", _context.getPackageName());

                View adView = (View) LayoutInflater.from(_context).inflate(layoutID, null);
                adView.setVisibility(View.INVISIBLE);

                layout.addView(adView);

                NativeAd nativeAd = new NativeAd(_context, NativeID);
                nativeAd.setAdListener(new AdListener() {

                    @Override
                    public void onError(Ad ad, AdError error) {
                        // Ad error callback
                        Log.d("FBAds", "NativeAd onError  " + error.getErrorMessage());
                    }

                    @Override
                    public void onAdLoaded(Ad ad) {
                        // Ad loaded callback
                        Log.d("FBAds", "NativeAd onAdLoaded");
                        NativeAd nativeAd = _dictFBNativeAd.get(ad.getPlacementId());
                        nativeAd.unregisterView();

                        _dictFBNativeAdReady.put(ad.getPlacementId(), "true");
                        View adView = _dictFBNativeAdView.get(ad.getPlacementId());

                        // Create native UI using the ad metadata.
                        int iconId = _context.getResources().getIdentifier("native_ad_icon", "id", _context.getPackageName());
                        int titleId = _context.getResources().getIdentifier("native_ad_title", "id", _context.getPackageName());
                        int mediaId = _context.getResources().getIdentifier("native_ad_media", "id", _context.getPackageName());
                        int socialId = _context.getResources().getIdentifier("native_ad_social_context", "id", _context.getPackageName());
                        int adchoiceId = _context.getResources().getIdentifier("ad_choices_container", "id", _context.getPackageName());
                        int bodyId = _context.getResources().getIdentifier("native_ad_body", "id", _context.getPackageName());
                        int callToActionId = _context.getResources().getIdentifier("native_ad_call_to_action", "id", _context.getPackageName());

                        ImageView nativeAdIcon = (ImageView) adView.findViewById(iconId);
                        TextView nativeAdTitle = (TextView) adView.findViewById(titleId);
                        MediaView nativeAdMedia = (MediaView) adView.findViewById(mediaId);
                        TextView nativeAdSocialContext = (TextView) adView.findViewById(socialId);
                        TextView nativeAdBody = (TextView) adView.findViewById(bodyId);
                        Button nativeAdCallToAction = (Button) adView.findViewById(callToActionId);

                        // Set the Text.
                        nativeAdTitle.setText(nativeAd.getAdTitle());
                        nativeAdSocialContext.setText(nativeAd.getAdSocialContext());
                        nativeAdBody.setText(nativeAd.getAdBody());
                        nativeAdCallToAction.setText(nativeAd.getAdCallToAction());

                        // Download and display the ad icon.
                        NativeAd.Image adIcon = nativeAd.getAdIcon();
                        NativeAd.downloadAndDisplayImage(adIcon, nativeAdIcon);

                        // Download and display the cover image.
                        nativeAdMedia.setNativeAd(nativeAd);

                        // Add the AdChoices icon
                        LinearLayout adChoicesContainer = (LinearLayout) adView.findViewById(adchoiceId);
                        AdChoicesView adChoicesView = new AdChoicesView(_context, nativeAd, true);
                        adChoicesContainer.addView(adChoicesView);
                    }

                    @Override
                    public void onAdClicked(Ad ad) {
                        // Ad clicked callback
                    }

                    @Override
                    public void onLoggingImpression(Ad ad) {
                        // Ad impression logged callback
                    }
                });

                _dictFBNativeAd.put(NativeID, nativeAd);
                _dictFBNativeAdView.put(NativeID, adView);
                _dictFBNativeAdReady.put(NativeID, "false");

                Log.d("FBAds", "Init NativeAD id = " + NativeID);
                nativeAd.loadAd();
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public void initFBAdsBanner(@NonNull String BannerID) {
        // not implement yet
    }

    @SuppressWarnings("WeakerAccess")
    public void cacheRewardedAd(@NonNull String adsID) {
    }

    @SuppressWarnings("WeakerAccess")
    public void cacheInterstitialAd(@NonNull String adsID) {
        initFBAdsInterstitial(adsID);
    }

    @SuppressWarnings("WeakerAccess")
    public void showBannerAd(@NonNull String adsID, int pos) {
    }

    @SuppressWarnings("WeakerAccess")
    public void hideBannerAd() {
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasInterstitialAd() {
        return _canShowInterstitialAd;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasRewardedAd() {
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public void showInterstitialAd() {
        if (_canShowInterstitialAd) {
            _context.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Log.d("FBAds", "Show Interstitial");
                    _interstitialAd.show();
                }
            });

            _canShowInterstitialAd = false;
        }
    }

    @SuppressWarnings("WeakerAccess")
    public void showRewardedAd() {
    }

    @SuppressWarnings("WeakerAccess")
    public void hideNativeAd(final @NonNull String adsID) {
        _context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                View adView = _dictFBNativeAdView.get(adsID);
                if(adView != null) {
                    adView.setVisibility(View.INVISIBLE);
                }
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public void showNativeAd(final @NonNull String adsID,final  int width,final  int height,final  int x,final  int y) {

        _context.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                View adView = _dictFBNativeAdView.get(adsID);
                if(adView != null) {
                    Log.d("FBAds", "Show NativeAds width  " + width + "  height " + height + "  x  " + x + "  y  " +y);
                    adView.setVisibility(View.VISIBLE);

                    DisplayMetrics metrics = new DisplayMetrics();
                    WindowManager wm = _context.getWindowManager();

                    int scale = 1;
                    if (wm != null) {
                        Display d = wm.getDefaultDisplay();
                        if (d != null) {
                            d.getMetrics(metrics);

                            scale = (int) metrics.density;
                        }
                    }

                    FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(width * scale, height * scale);
                    params.leftMargin = x;
                    params.topMargin = y;
                    adView.setLayoutParams(params);
                }
            }
        });
    }

    @SuppressWarnings("WeakerAccess")
    public boolean hasNativeAd(final @NonNull String adsID)
    {
        return _dictFBNativeAdReady.get(adsID).compareTo("true") == 0;
    }

}
