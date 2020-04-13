package com.ee.admob;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Point;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.RatingBar;
import android.widget.TextView;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.core.IMessageBridge;
import com.ee.core.MessageBridge;
import com.ee.core.Logger;
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

/**
 * Created by Zinge on 10/16/17.
 */

class AdMobNativeAd extends AdListener implements IAdView {
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
    private AdViewHelper        _helper;
    private IMessageBridge      _bridge;

    public AdMobNativeAd(@NonNull Context context, @Nullable Activity activity, @NonNull String adId, @NonNull String layoutName, @NonNull Map<String, String> identifiers) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _adId = adId;
        _layoutName = layoutName;
        _identifiers = identifiers;
        _isAdLoaded = false;
        _helper = new AdViewHelper("AdMobNativeAd", adId);
        _bridge = MessageBridge.getInstance();

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
        _helper = null;
        _bridge = null;
    }

    private String kOnLoaded() {
        return "AdMobNativeAd_onLoaded_" + _adId;
    }

    private String kOnFailedToLoad() {
        return "AdMobNativeAd_onFailedToLoad_" + _adId;
    }

    private String kOnClicked() {
        return "AdMobNativeAd_onClicked_" + _adId;
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
        _adLoader = new AdLoader.Builder(_context, _adId).forUnifiedNativeAd(new UnifiedNativeAd.OnUnifiedNativeAdLoadedListener() {
            @Override
            public void onUnifiedNativeAdLoaded(UnifiedNativeAd nativeAppInstallAd) {
                _logger.info("onAppInstallAdLoaded");
                int layoutId = _context.getResources().getIdentifier(_layoutName, "layout", _context.getPackageName());
                UnifiedNativeAdView adView = (UnifiedNativeAdView) LayoutInflater.from(_context).inflate(layoutId, null, false);
                populateUnifiedNativeAdView(nativeAppInstallAd, adView);
                _nativeAdPlaceholder.removeAllViews();

                ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
                _nativeAdPlaceholder.addView(adView, params);

                _isAdLoaded = true;
                _bridge.callCpp(kOnLoaded());
            }
        }).withAdListener(this).build();
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

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
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
        /*
          Cocos2d-x issue.
        // FIXME: 11/23/17 quickfix bug onCreate again
        if(_nativeAdPlaceholder.getParent() != null)
        {
            ((FrameLayout)_nativeAdPlaceholder.getParent()).removeView(_nativeAdPlaceholder);
        }
        // end quickfix bug onCreate again
        */
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
        _isAdLoaded = false;
        AdRequest.Builder builder = new AdRequest.Builder();
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
    public boolean isVisible() {
        return AdViewHelper.isVisible(_nativeAdPlaceholder);
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
        return resources.getIdentifier(_identifiers.get(identifier), "id", _context.getPackageName());
    }

    private void populateUnifiedNativeAdView(final UnifiedNativeAd nativeAppInstallAd, final UnifiedNativeAdView adView) {
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
        processView(adView, k__body, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                adView.setBodyView(view);
                view.setText(nativeAppInstallAd.getBody());
            }
        });

        processView(adView, k__call_to_action, new ViewProcessor<Button>() {
            @Override
            public void process(Button view) {
                adView.setCallToActionView(view);
                view.setText(nativeAppInstallAd.getCallToAction());
            }
        });

        processView(adView, k__headline, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                adView.setHeadlineView(view);
                view.setText(nativeAppInstallAd.getHeadline());
            }
        });

        processView(adView, k__icon, new ViewProcessor<ImageView>() {
            @Override
            public void process(ImageView view) {
                adView.setIconView(view);
                if (nativeAppInstallAd.getIcon() != null) {
                    view.setImageDrawable(nativeAppInstallAd.getIcon().getDrawable());
                }
            }
        });

        // Apps can check the VideoController's hasVideoContent property to determine if the
        // NativeAppInstallAd has a video asset.
        if (vc.hasVideoContent()) {
            processView(adView, k__image, new ViewProcessor<ImageView>() {
                @Override
                public void process(ImageView view) {
                    view.setVisibility(View.GONE);
                }
            });
            processView(adView, k__media, new ViewProcessor<MediaView>() {
                @Override
                public void process(MediaView view) {
                    adView.setMediaView(view);
                }
            });
        } else {
            processView(adView, k__image, new ViewProcessor<ImageView>() {
                @Override
                public void process(ImageView view) {
                    List<NativeAd.Image> images = nativeAppInstallAd.getImages();
                    view.setVisibility(View.GONE);
                    for (NativeAd.Image image : images) {
                        if (image != null) {
                            view.setVisibility(View.VISIBLE);
                            view.setImageDrawable(image.getDrawable());
                            break;
                        }
                    }
                }
            });
            processView(adView, k__media, new ViewProcessor<MediaView>() {
                @Override
                public void process(MediaView view) {
                    view.setVisibility(View.GONE);
                }
            });
        }

        // These assets aren't guaranteed to be in every NativeAppInstallAd, so it's important to
        // check before trying to display them.
        processView(adView, k__price, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                adView.setPriceView(view);
                CharSequence price = nativeAppInstallAd.getPrice();
                if (price != null) {
                    view.setVisibility(View.VISIBLE);
                    view.setText(price);
                } else {
                    view.setVisibility(View.INVISIBLE);
                }
            }
        });

        processView(adView, k__star_rating, new ViewProcessor<RatingBar>() {
            @Override
            public void process(RatingBar view) {
                adView.setStarRatingView(view);
                Double starRating = nativeAppInstallAd.getStarRating();
                if (starRating != null) {
                    view.setRating(starRating.floatValue());
                    view.setVisibility(View.VISIBLE);
                } else {
                    view.setVisibility(View.INVISIBLE);
                }
            }
        });

        processView(adView, k__store, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                adView.setStoreView(view);
                CharSequence store = nativeAppInstallAd.getStore();
                if (store != null) {
                    view.setVisibility(View.VISIBLE);
                    view.setText(store);
                } else {
                    view.setVisibility(View.INVISIBLE);
                }
            }
        });

        // Assign native ad object to the native view.
        adView.setNativeAd(nativeAppInstallAd);
    }

    private interface ViewProcessor<T extends View> {
        void process(T view);
    }

    @SuppressWarnings("unchecked")
    private <T extends View> void processView(@NonNull View view, @NonNull String key, @NonNull ViewProcessor<T> processor) {
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
        Utils.checkMainThread();
    }

    @Override
    public void onAdFailedToLoad(int errorCode) {
        _logger.info("onAdFailedToLoad: code = " + errorCode);
        Utils.checkMainThread();
        _bridge.callCpp(kOnFailedToLoad(), String.valueOf(errorCode));
    }

    @Override
    public void onAdLeftApplication() {
        _logger.info("onAdLeftApplication");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClicked());
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
