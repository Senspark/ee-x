package com.ee.admob;

import android.app.Activity;
import android.content.res.Resources;
import android.graphics.Point;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.TextView;

import com.ee.ads.AdViewHelper;
import com.ee.ads.AdViewInterface;
import com.ee.core.Logger;
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

    private static final String k__headline       = "headline";
    private static final String k__body           = "body";
    private static final String k__call_to_action = "call_to_action";
    private static final String k__icon           = "icon";
    private static final String k__price          = "price";
    private static final String k__star_rating    = "star_rating";
    private static final String k__media          = "media";
    private static final String k__main_image     = "main_image";

    private Activity            _activity;
    private String              _adId;
    private String              _layoutName;
    private Map<String, String> _identifiers;
    private boolean             _isAdLoaded;
    private AdLoader            _adLoader;
    private FrameLayout         _nativeAdPlaceholder;
    private List<String>        _testDevices;
    private AdViewHelper        _helper;

    public AdMobNativeAd(@NonNull Activity activity, @NonNull String adId,
                         @NonNull String layoutName, @NonNull Map<String, String> identifiers,
                         @NonNull List<String> testDevices) {
        Utils.checkMainThread();
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

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        destroyInternalAd();
        destroyView();
        _layoutName = null;
        _identifiers = null;
        _adId = null;
        _activity = null;
        _testDevices = null;
        _helper = null;
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
        _adLoader = new AdLoader.Builder(_activity, _adId)
            .forAppInstallAd(new NativeAppInstallAd.OnAppInstallAdLoadedListener() {
                @Override
                public void onAppInstallAdLoaded(NativeAppInstallAd nativeAppInstallAd) {
                    FrameLayout rootView = Utils.getRootView(_activity);
                    int layoutId = rootView
                        .getResources()
                        .getIdentifier(_layoutName, "layout", _activity.getPackageName());
                    NativeAppInstallAdView adView = (NativeAppInstallAdView) LayoutInflater
                        .from(_activity)
                        .inflate(layoutId, null);
                    populateAppInstallAdView(nativeAppInstallAd, adView);
                    _nativeAdPlaceholder.removeAllViews();
                    _nativeAdPlaceholder.addView(adView);
                }
            })
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
        _nativeAdPlaceholder = new FrameLayout(_activity);
    }

    private void destroyView() {
        assert _nativeAdPlaceholder != null;
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
        AdRequest.Builder builder = new AdRequest.Builder();
        for (String hash : _testDevices) {
            builder.addTestDevice(hash);
        }
        _adLoader.loadAd(builder.build());
    }

    @NonNull
    @Override
    public Point getPosition() {
        Utils.checkMainThread();
        int p[] = new int[2];
        _nativeAdPlaceholder.getLocationInWindow(p);
        return new Point(p[0], p[1]);
    }

    @Override
    public void setPosition(@NonNull Point position) {
        Utils.checkMainThread();
        FrameLayout.LayoutParams params =
            (FrameLayout.LayoutParams) _nativeAdPlaceholder.getLayoutParams();
        params.leftMargin = position.x;
        params.topMargin = position.y;
        _nativeAdPlaceholder.setLayoutParams(params);
    }

    @NonNull
    @Override
    public Point getSize() {
        Utils.checkMainThread();
        return new Point(_nativeAdPlaceholder.getWidth(), _nativeAdPlaceholder.getHeight());
    }

    @Override
    public void setSize(@NonNull Point size) {
        Utils.checkMainThread();
        ViewGroup.LayoutParams params = _nativeAdPlaceholder.getLayoutParams();
        params.width = size.x;
        params.height = size.y;
        _nativeAdPlaceholder.setLayoutParams(params);
    }

    @Override
    public void setVisible(boolean visible) {
        Utils.checkMainThread();
        if (visible) {
            _nativeAdPlaceholder.setVisibility(View.VISIBLE);
        } else {
            _nativeAdPlaceholder.setVisibility(View.INVISIBLE);
        }
    }

    private int getIdentifier(@NonNull String identifier) {
        Resources resources = _activity.getResources();
        return resources.getIdentifier(_identifiers.get(identifier), "id",
            _activity.getPackageName());
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

        adView.setHeadlineView(adView.findViewById(getIdentifier(k__headline)));
        adView.setBodyView(adView.findViewById(getIdentifier(k__body)));
        adView.setCallToActionView(adView.findViewById(getIdentifier(k__call_to_action)));
        adView.setIconView(adView.findViewById(getIdentifier(k__icon)));
        adView.setPriceView(adView.findViewById(getIdentifier(k__price)));
        adView.setStarRatingView(adView.findViewById(getIdentifier(k__star_rating)));
        adView.setStoreView(adView.findViewById(getIdentifier(k__star_rating)));

        // Some assets are guaranteed to be in every NativeAppInstallAd.
        ((TextView) adView.getHeadlineView()).setText(nativeAppInstallAd.getHeadline());
        ((TextView) adView.getBodyView()).setText(nativeAppInstallAd.getBody());
        ((Button) adView.getCallToActionView()).setText(nativeAppInstallAd.getCallToAction());
        ((ImageView) adView.getIconView()).setImageDrawable(
            nativeAppInstallAd.getIcon().getDrawable());

        MediaView mediaView = adView.findViewById(getIdentifier(k__media));
        ImageView mainImageView = adView.findViewById(getIdentifier(k__main_image));

        // Apps can check the VideoController's hasVideoContent property to determine if the
        // NativeAppInstallAd has a video asset.
        if (vc.hasVideoContent()) {
            adView.setMediaView(mediaView);
            mainImageView.setVisibility(View.GONE);
        } else {
            adView.setImageView(mainImageView);
            mediaView.setVisibility(View.GONE);

            // At least one image is guaranteed.
            List<NativeAd.Image> images = nativeAppInstallAd.getImages();
            mainImageView.setImageDrawable(images.get(0).getDrawable());
        }

        // These assets aren't guaranteed to be in every NativeAppInstallAd, so it's important to
        // check before trying to display them.
        if (nativeAppInstallAd.getPrice() == null) {
            adView.getPriceView().setVisibility(View.INVISIBLE);
        } else {
            adView.getPriceView().setVisibility(View.VISIBLE);
            ((TextView) adView.getPriceView()).setText(nativeAppInstallAd.getPrice());
        }

        if (nativeAppInstallAd.getStore() == null) {
            adView.getStoreView().setVisibility(View.INVISIBLE);
        } else {
            adView.getStoreView().setVisibility(View.VISIBLE);
            ((TextView) adView.getStoreView()).setText(nativeAppInstallAd.getStore());
        }

        if (nativeAppInstallAd.getStarRating() == null) {
            adView.getStarRatingView().setVisibility(View.INVISIBLE);
        } else {
            ((RatingBar) adView.getStarRatingView()).setRating(
                nativeAppInstallAd.getStarRating().floatValue());
            adView.getStarRatingView().setVisibility(View.VISIBLE);
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
