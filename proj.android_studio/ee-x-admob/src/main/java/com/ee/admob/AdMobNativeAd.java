package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.TextView;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
import com.ee.core.internal.MessageBridge;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdLoader;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.VideoController;
import com.google.android.gms.ads.formats.MediaView;
import com.google.android.gms.ads.formats.NativeAd;
import com.google.android.gms.ads.formats.NativeAppInstallAd;
import com.google.android.gms.ads.formats.NativeAppInstallAdView;

import java.util.List;
import java.util.Map;

/**
 * Created by Zinge on 10/16/17.
 */

class AdMobNativeAd extends AdListener implements AdViewInterface {
    private static final Logger _logger = new Logger(AdMobNativeAd.class.getName());

    private static final String k__body           = "body";
    private static final String k__call_to_action = "call_to_action";
    private static final String k__headline       = "headline";
    private static final String k__icon           = "icon";
    private static final String k__image          = "image";
    private static final String k__media          = "media";
    private static final String k__price          = "price";
    private static final String k__star_rating    = "star_rating";
    private static final String k__store          = "store";

    private Context             _context;
    private Activity            _activity;
    private String              _adId;
    private String              _layoutName;
    private Map<String, String> _identifiers;
    private boolean             _isAdLoaded;
    private AdLoader            _adLoader;
    private FrameLayout         _nativeAdPlaceholder;
    private List<String>        _testDevices;
    private AdViewHelper        _helper;

    public AdMobNativeAd(@NonNull Context context, @Nullable Activity activity,
                         @NonNull String adId, @NonNull String layoutName,
                         @NonNull Map<String, String> identifiers,
                         @NonNull List<String> testDevices) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _adId = adId;
        _layoutName = layoutName;
        _identifiers = identifiers;
        _isAdLoaded = false;
        _testDevices = testDevices;
        _helper = new AdViewHelper("AdMobNativeAd", adId);
        createInternalAd();
        createView();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onDestroy(@NonNull Activity activity) {
        assert _activity == activity;
        removeFromActivity(activity);
        _activity = null;
    }

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        destroyView();
        _layoutName = null;
        _identifiers = null;
        _adId = null;
        _context = null;
        _testDevices = null;
        _helper = null;
    }

    private String k__onLoaded() {
        return "AdMobNativeAd_onLoaded_" + _adId;
    }

    private String k__onFailedToLoad() {
        return "AdMobNativeAd_onFailedToLoad_" + _adId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers(this);
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createInternalAd() {
        if (_adLoader != null) {
            return false;
        }
        _isAdLoaded = false;
        _adLoader = new AdLoader.Builder(_context, _adId)
            .forAppInstallAd(new NativeAppInstallAd.OnAppInstallAdLoadedListener() {
                @Override
                public void onAppInstallAdLoaded(NativeAppInstallAd nativeAppInstallAd) {
                    _logger.info("onAppInstallAdLoaded");
                    int layoutId = _context
                        .getResources()
                        .getIdentifier(_layoutName, "layout", _context.getPackageName());
                    NativeAppInstallAdView adView = (NativeAppInstallAdView) LayoutInflater
                        .from(_context)
                        .inflate(layoutId, null);
                    populateAppInstallAdView(nativeAppInstallAd, adView);
                    _nativeAdPlaceholder.removeAllViews();
                    _nativeAdPlaceholder.addView(adView);

                    _isAdLoaded = true;
                    MessageBridge bridge = MessageBridge.getInstance();
                    bridge.callCpp(k__onLoaded());
                }
            })
            .withAdListener(this)
            .build();
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyInternalAd() {
        if (_adLoader == null) {
            return false;
        }
        _isAdLoaded = false;
        _adLoader = null;
        return true;
    }

    private void createView() {
        assert _nativeAdPlaceholder == null;
        FrameLayout layout = new FrameLayout(_context);

        FrameLayout.LayoutParams params =
            new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        layout.setLayoutParams(params);

        _nativeAdPlaceholder = layout;
        if (_activity != null) {
            addToActivity(_activity);
        }
    }

    private void destroyView() {
        Utils.checkMainThread();
        assert _nativeAdPlaceholder != null;
        removeFromActivity(_activity);
        _nativeAdPlaceholder = null;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_nativeAdPlaceholder);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_nativeAdPlaceholder);
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _adLoader != null && _isAdLoaded;
    }

    @Override
    public void load() {
        if (_adLoader == null) {
            return;
        }
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        for (String hash : _testDevices) {
            builder.addTestDevice(hash);
        }
        _logger.info("load");
        _adLoader.loadAd(builder.build());
    }

    @NonNull
    @Override
    public Point getPosition() {
        return AdViewHelper.getPosition(_nativeAdPlaceholder);
    }

    @Override
    public void setPosition(@NonNull Point position) {
        AdViewHelper.setPosition(position, _nativeAdPlaceholder);
    }

    @NonNull
    @Override
    public Point getSize() {
        return AdViewHelper.getSize(_nativeAdPlaceholder);
    }

    @Override
    public void setSize(@NonNull Point size) {
        AdViewHelper.setSize(size, _nativeAdPlaceholder);
    }

    @Override
    public void setVisible(boolean visible) {
        AdViewHelper.setVisible(visible, _nativeAdPlaceholder);
    }

    private int getIdentifier(@NonNull String identifier) {
        if (!_identifiers.containsKey(identifier)) {
            return 0;
        }
        Resources resources = _context.getResources();
        return resources.getIdentifier(_identifiers.get(identifier), "id",
            _context.getPackageName());
    }

    private void populateAppInstallAdView(NativeAppInstallAd nativeAppInstallAd,
                                          NativeAppInstallAdView adView) {
        // Get the video controller for the ad. One will always be provided, even if the ad doesn't
        // have a video asset.
        VideoController vc = nativeAppInstallAd.getVideoController();

        // Create a new VideoLifecycleCallbacks object and pass it to the VideoController. The
        // VideoController will call methods on this object when events occur in the video
        // lifecycle.
        vc.setVideoLifecycleCallbacks(new VideoController.VideoLifecycleCallbacks() {
            public void onVideoEnd() {
                // Publishers should allow native ads to complete video playback before refreshing
                // or replacing them with another ad in the same UI location.
                super.onVideoEnd();
            }
        });

        adView.setBodyView(adView.findViewById(getIdentifier(k__body)));
        adView.setCallToActionView(adView.findViewById(getIdentifier(k__call_to_action)));
        adView.setHeadlineView(adView.findViewById(getIdentifier(k__headline)));
        adView.setIconView(adView.findViewById(getIdentifier(k__icon)));
        adView.setPriceView(adView.findViewById(getIdentifier(k__price)));
        adView.setStarRatingView(adView.findViewById(getIdentifier(k__star_rating)));
        adView.setStoreView(adView.findViewById(getIdentifier(k__store)));

        // Some assets are guaranteed to be in every NativeAppInstallAd.
        TextView bodyView = (TextView) adView.getBodyView();
        Button callToActionView = (Button) adView.getCallToActionView();
        TextView headlineView = (TextView) adView.getHeadlineView();
        ImageView iconView = (ImageView) adView.getIconView();

        bodyView.setText(nativeAppInstallAd.getBody());
        callToActionView.setText(nativeAppInstallAd.getCallToAction());
        headlineView.setText(nativeAppInstallAd.getHeadline());
        iconView.setImageDrawable(nativeAppInstallAd.getIcon().getDrawable());

        ImageView imageView = adView.findViewById(getIdentifier(k__image));
        MediaView mediaView = adView.findViewById(getIdentifier(k__media));

        // Apps can check the VideoController's hasVideoContent property to determine if the
        // NativeAppInstallAd has a video asset.
        if (vc.hasVideoContent()) {
            adView.setMediaView(mediaView);
            imageView.setVisibility(View.GONE);
        } else {
            adView.setImageView(imageView);
            mediaView.setVisibility(View.GONE);

            // At least one image is guaranteed.
            List<NativeAd.Image> images = nativeAppInstallAd.getImages();
            imageView.setImageDrawable(images.get(0).getDrawable());
        }

        // These assets aren't guaranteed to be in every NativeAppInstallAd, so it's important to
        // check before trying to display them.
        CharSequence price = nativeAppInstallAd.getPrice();
        TextView priceView = (TextView) adView.getPriceView();
        if (price != null) {
            priceView.setVisibility(View.VISIBLE);
            priceView.setText(price);
        } else {
            priceView.setVisibility(View.INVISIBLE);
        }

        Double starRating = nativeAppInstallAd.getStarRating();
        RatingBar starRatingView = (RatingBar) adView.getStarRatingView();
        if (starRating != null) {
            starRatingView.setRating(starRating.floatValue());
            starRatingView.setVisibility(View.VISIBLE);
        } else {
            starRatingView.setVisibility(View.INVISIBLE);
        }

        CharSequence store = nativeAppInstallAd.getStore();
        TextView storeView = (TextView) adView.getStoreView();
        if (store != null) {
            storeView.setVisibility(View.VISIBLE);
            storeView.setText(store);
        } else {
            storeView.setVisibility(View.INVISIBLE);
        }

        // Assign native ad object to the native view.
        adView.setNativeAd(nativeAppInstallAd);
    }

    @Override
    public void onAdClosed() {
        _logger.info("onAdClosed");
        Utils.checkMainThread();
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();
        MessageBridge bridge = MessageBridge.getInstance();
        bridge.callCpp(k__onFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Utils.checkMainThread();
    }

    @Override
    public void onAdOpened() {
        _logger.info("onAdOpened");
        Utils.checkMainThread();
    }

    @Override
    public void onAdLoaded() {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        _isAdLoaded = true;
    }

    @Override
    public void onAdClicked() {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
    }

    @Override
    public void onAdImpression() {
        _logger.info("onAdImpression");
        Utils.checkMainThread();
    }
}
