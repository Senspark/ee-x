package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Point;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.ads.MessageHelper;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.internal.Thread;
import com.ee.core.internal.Utils;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdLoader;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.VideoController;
import com.google.android.gms.ads.formats.MediaView;
import com.google.android.gms.ads.formats.NativeAd;
import com.google.android.gms.ads.formats.UnifiedNativeAd;
import com.google.android.gms.ads.formats.UnifiedNativeAdView;

import java.util.List;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/16/17.
 */

class AdMobNativeAd extends AdListener implements IAdView {
    private static final Logger _logger = new Logger(AdMobNativeAd.class.getName());

    private static final String k__body = "body";
    private static final String k__call_to_action = "call_to_action";
    private static final String k__headline = "headline";
    private static final String k__icon = "icon";
    private static final String k__image = "image";
    private static final String k__media = "media";
    private static final String k__price = "price";
    private static final String k__star_rating = "star_rating";
    private static final String k__store = "store";

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private String _adId;
    private String _layoutName;
    private Map<String, String> _identifiers;
    private MessageHelper _messageHelper;
    private AdViewHelper _helper;
    private ViewHelper _viewHelper;
    private boolean _isLoaded;
    private AdLoader _ad;
    private FrameLayout _view;

    AdMobNativeAd(@NonNull Context context,
                  @Nullable Activity activity,
                  @NonNull IMessageBridge bridge,
                  @NonNull String adId,
                  @NonNull String layoutName,
                  @NonNull Map<String, String> identifiers) {
        _logger.info("constructor: adId = %s", adId);
        Thread.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = bridge;
        _adId = adId;
        _layoutName = layoutName;
        _identifiers = identifiers;
        _messageHelper = new MessageHelper("AdMobNativeAd", adId);
        _helper = new AdViewHelper(_bridge, this, _messageHelper);

        createInternalAd();
        createView();
        registerHandlers();
    }

    void onCreate(@NonNull Activity activity) {
        _activity = activity;
        addToActivity(activity);
    }

    void onDestroy(@NonNull Activity activity) {
        assertThat(_activity).isEqualTo(activity);
        removeFromActivity(activity);
        _activity = null;
    }

    void destroy() {
        _logger.info("destroy: adId = " + _adId);
        Thread.checkMainThread();
        deregisterHandlers();
        destroyView();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _layoutName = null;
        _identifiers = null;
        _messageHelper = null;
        _helper = null;
    }

    private void registerHandlers() {
        _helper.registerHandlers();
    }

    private void deregisterHandlers() {
        _helper.deregisterHandlers();
    }

    private boolean createInternalAd() {
        Thread.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _isLoaded = false;
        _ad = new AdLoader.Builder(_context, _adId)
            .forUnifiedNativeAd(nativeAppInstallAd -> {
                _logger.info("onAppInstallAdLoaded");
                int layoutId = _context.getResources().getIdentifier(_layoutName, "layout", _context.getPackageName());
                UnifiedNativeAdView adView = (UnifiedNativeAdView) LayoutInflater
                    .from(_context)
                    .inflate(layoutId, null, false);
                populateUnifiedNativeAdView(nativeAppInstallAd, adView);
                _view.removeAllViews();

                ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT,
                    ViewGroup.LayoutParams.MATCH_PARENT);
                _view.addView(adView, params);

                _isLoaded = true;
                _bridge.callCpp(_messageHelper.onLoaded());
            }).withAdListener(this).build();
        return true;
    }

    private boolean destroyInternalAd() {
        Thread.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _isLoaded = false;
        _ad = null;
        return true;
    }

    private void createView() {
        Thread.checkMainThread();
        assertThat(_view).isNull();
        FrameLayout layout = new FrameLayout(_context);

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        layout.setLayoutParams(params);

        _view = layout;
        _viewHelper = new ViewHelper(_view);

        if (_activity != null) {
            addToActivity(_activity);
        }
    }

    private void destroyView() {
        Thread.checkMainThread();
        assertThat(_view).isNotNull();
        if (_activity != null) {
            removeFromActivity(_activity);
        }
        _viewHelper = null;
        _view = null;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_view);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_view);
    }

    @Override
    public boolean isLoaded() {
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        return _isLoaded;
    }

    @Override
    public void load() {
        Thread.checkMainThread();
        assertThat(_ad).isNotNull();
        _logger.info("load");
        AdRequest.Builder builder = new AdRequest.Builder();
        _ad.loadAd(builder.build());
    }

    @NonNull
    @Override
    public Point getPosition() {
        return _viewHelper.getPosition();
    }

    @Override
    public void setPosition(@NonNull Point position) {
        _viewHelper.setPosition(position);
    }

    @NonNull
    @Override
    public Point getSize() {
        return _viewHelper.getSize();
    }

    @Override
    public void setSize(@NonNull Point size) {
        _viewHelper.setSize(size);
    }

    @Override
    public boolean isVisible() {
        return _viewHelper.isVisible();
    }

    @Override
    public void setVisible(boolean visible) {
        _viewHelper.setVisible(visible);
    }

    private int getIdentifier(@NonNull String identifier) {
        if (!_identifiers.containsKey(identifier)) {
            return 0;
        }
        Resources resources = _context.getResources();
        return resources.getIdentifier(_identifiers.get(identifier), "id", _context.getPackageName());
    }

    private void populateUnifiedNativeAdView(UnifiedNativeAd nativeAppInstallAd,
                                             UnifiedNativeAdView adView) {
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

        // Some assets are guaranteed to be in every NativeAppInstallAd.
        processView(adView, k__body, (ViewProcessor<TextView>) view -> {
            adView.setBodyView(view);
            view.setText(nativeAppInstallAd.getBody());
        });

        processView(adView, k__call_to_action, (ViewProcessor<Button>) view -> {
            adView.setCallToActionView(view);
            view.setText(nativeAppInstallAd.getCallToAction());
        });

        processView(adView, k__headline, (ViewProcessor<TextView>) view -> {
            adView.setHeadlineView(view);
            view.setText(nativeAppInstallAd.getHeadline());
        });

        processView(adView, k__icon, (ViewProcessor<ImageView>) view -> {
            adView.setIconView(view);
            if (nativeAppInstallAd.getIcon() != null) {
                view.setImageDrawable(nativeAppInstallAd.getIcon().getDrawable());
            }
        });

        // Apps can check the VideoController's hasVideoContent property to determine if the
        // NativeAppInstallAd has a video asset.
        if (vc.hasVideoContent()) {
            processView(adView, k__image, (ViewProcessor<ImageView>) view ->
                view.setVisibility(View.GONE));
            processView(adView, k__media, adView::setMediaView);
        } else {
            processView(adView, k__image, (ViewProcessor<ImageView>) view -> {
                List<NativeAd.Image> images = nativeAppInstallAd.getImages();
                view.setVisibility(View.GONE);
                for (NativeAd.Image image : images) {
                    if (image != null) {
                        view.setVisibility(View.VISIBLE);
                        view.setImageDrawable(image.getDrawable());
                        break;
                    }
                }
            });
            processView(adView, k__media, (ViewProcessor<MediaView>) view ->
                view.setVisibility(View.GONE));
        }

        // These assets aren't guaranteed to be in every NativeAppInstallAd, so it's important to
        // check before trying to display them.
        processView(adView, k__price, (ViewProcessor<TextView>) view -> {
            adView.setPriceView(view);
            CharSequence price = nativeAppInstallAd.getPrice();
            if (price != null) {
                view.setVisibility(View.VISIBLE);
                view.setText(price);
            } else {
                view.setVisibility(View.INVISIBLE);
            }
        });

        processView(adView, k__star_rating, (ViewProcessor<RatingBar>) view -> {
            adView.setStarRatingView(view);
            Double starRating = nativeAppInstallAd.getStarRating();
            if (starRating != null) {
                view.setRating(starRating.floatValue());
                view.setVisibility(View.VISIBLE);
            } else {
                view.setVisibility(View.INVISIBLE);
            }
        });

        processView(adView, k__store, (ViewProcessor<TextView>) view -> {
            adView.setStoreView(view);
            CharSequence store = nativeAppInstallAd.getStore();
            if (store != null) {
                view.setVisibility(View.VISIBLE);
                view.setText(store);
            } else {
                view.setVisibility(View.INVISIBLE);
            }
        });

        // Assign native ad object to the native view.
        adView.setNativeAd(nativeAppInstallAd);
    }

    private interface ViewProcessor<T extends View> {
        void process(T view);
    }

    @SuppressWarnings("unchecked")
    private <T extends View> void processView(@NonNull View view,
                                              @NonNull String key,
                                              @NonNull ViewProcessor<T> processor) {
        int id = getIdentifier(key);
        if (id == 0) {
            _logger.error("Can not find identifier for key: " + key);
            return;
        }
        View subView = view.findViewById(id);
        if (subView == null) {
            _logger.error("Can not find view for key: " + key);
            return;
        }
        processor.process((T) subView);
    }

    @Override
    public void onAdClosed() {
        _logger.info("onAdClosed");
        Thread.checkMainThread();
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Thread.checkMainThread();
        _bridge.callCpp(_messageHelper.onClicked());
    }

    @Override
    public void onAdOpened() {
        _logger.info("onAdOpened");
        Thread.checkMainThread();
    }

    @Override
    public void onAdLoaded() {
        _logger.info("onAdLoaded");
        Thread.checkMainThread();
        // There is one important difference between the way AdListener objects work with native ads
        // and the way they work with banners and interstitials. Because the AdLoader has its own
        // format-specific listeners (i.e., UnifiedNativeAd.OnUnifiedNativeAdLoadedListener) to use
        // when an ad has loaded, the onAdLoaded() method from AdListener is not called when a
        // native ad loads successfully.
    }

    @Override
    public void onAdClicked() {
        _logger.info("onAdClicked");
        Thread.checkMainThread();
    }

    @Override
    public void onAdImpression() {
        _logger.info("onAdImpression");
        Thread.checkMainThread();
    }
}
