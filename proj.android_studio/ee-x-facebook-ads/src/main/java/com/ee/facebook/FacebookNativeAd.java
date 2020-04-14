package com.ee.facebook;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Point;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.ads.AdViewHelper;
import com.ee.ads.IAdView;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.MessageHandler;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdOptionsView;
import com.facebook.ads.MediaView;
import com.facebook.ads.NativeAd;
import com.facebook.ads.NativeAdListener;

import java.util.Arrays;
import java.util.Map;

/**
 * Created by Zinge on 10/9/17.
 */

class FacebookNativeAd implements NativeAdListener, IAdView {
    private static final String k__ad_choices = "ad_choices";
    private static final String k__body = "body";
    private static final String k__call_to_action = "call_to_action";
    private static final String k__icon = "icon";
    private static final String k__media = "media";
    private static final String k__social_context = "social_context";
    private static final String k__title = "title";
    private static final String k__cover = "cover";

    private static final String k__tag = "FacebookNativeAd";

    private static final Logger _logger = new Logger(FacebookNativeAd.class.getName());

    private IMessageBridge _bridge;
    private NativeAd _nativeAd;
    private View _nativeAdView;
    private boolean _isAdLoaded;
    private Context _context;
    private Activity _activity;
    private String _adId;
    private String _layoutName;
    private Map<String, String> _identifiers;
    private AdViewHelper _helper;
    private ViewHelper _viewHelper;

    public FacebookNativeAd(@NonNull Context context, @Nullable Activity activity, @NonNull String adId, @NonNull String layoutName, @NonNull Map<String, String> identifiers) {
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _layoutName = layoutName;
        _identifiers = identifiers;
        _helper = new AdViewHelper(_bridge, this, k__tag, adId);

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
        destroyView();
        destroyInternalAd();
        _context = null;
        _bridge = null;
        _adId = null;
        _layoutName = null;
        _identifiers = null;
        _helper = null;
    }

    @NonNull
    private String kCreateInternalAd() {
        return k__tag + "_createInternalAd_" + _adId;
    }

    @NonNull
    private String kDestroyInternalAd() {
        return k__tag + "_destroyInternalAd_" + _adId;
    }

    @NonNull
    private String kOnLoaded() {
        return k__tag + "_onLoaded_" + _adId;
    }

    @NonNull
    private String kOnFailedToLoad() {
        return k__tag + "_onFailedToLoad_" + _adId;
    }

    @NonNull
    private String kOnClicked() {
        return k__tag + "_onClicked_" + _adId;
    }

    private void registerHandlers() {
        Utils.checkMainThread();
        _helper.registerHandlers();

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(createInternalAd());
            }
        }, kCreateInternalAd());

        _bridge.registerHandler(new MessageHandler() {
            @NonNull
            @Override
            public String handle(@NonNull String message) {
                return Utils.toString(destroyInternalAd());
            }
        }, kDestroyInternalAd());
    }

    private void deregisterHandlers() {
        Utils.checkMainThread();
        _helper.deregisterHandlers();

        _bridge.deregisterHandler(kCreateInternalAd());
        _bridge.deregisterHandler(kDestroyInternalAd());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_nativeAd != null) {
            return false;
        }
        _isAdLoaded = false;
        NativeAd nativeAd = new NativeAd(_context, _adId);
        nativeAd.setAdListener(this);
        _nativeAd = nativeAd;
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_nativeAd == null) {
            return false;
        }
        _isAdLoaded = false;
        _nativeAd.unregisterView();
        _nativeAd.destroy();
        _nativeAd = null;
        return true;
    }

    private void createView() {
        Utils.checkMainThread();
        int layoutId = _context.getResources().getIdentifier(_layoutName, "layout", _context.getPackageName());
        View nativeAdView = LayoutInflater.from(_context).inflate(layoutId, null, false);
        nativeAdView.setVisibility(View.INVISIBLE);

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        nativeAdView.setLayoutParams(params);

        _nativeAdView = nativeAdView;
        _viewHelper = new ViewHelper(_nativeAdView);

        if (_activity != null) {
            addToActivity(_activity);
        }
    }

    private void destroyView() {
        Utils.checkMainThread();
        if (_activity != null) {
            removeFromActivity(_activity);
        }
        _viewHelper = null;
        _nativeAdView = null;
    }

    private void addToActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.addView(_nativeAdView);
    }

    private void removeFromActivity(@NonNull Activity activity) {
        FrameLayout rootView = Utils.getRootView(activity);
        rootView.removeView(_nativeAdView);
    }

    @Override
    public boolean isLoaded() {
        Utils.checkMainThread();
        return _nativeAd != null && _isAdLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        if (_nativeAd == null) {
            return;
        }
        _logger.info("load");

        ///
        /// Audience Network supports pre-caching video or image assets which enables
        /// the MediaView to play videos immediately after nativeAd finishes loading.
        /// https://developers.facebook.com/docs/audience-network/android-native#mediaview
        ///
        _nativeAd.loadAd(NativeAd.MediaCacheFlag.ALL);
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

    private interface ViewProcessor<T extends View> {
        void process(T view);
    }

    @SuppressWarnings("unchecked")
    private <T extends View> boolean processView(@NonNull View view, @NonNull String key, @NonNull ViewProcessor<T> processor) {
        int id = getIdentifier(key);
        if (id == 0) {
            _logger.error("Can not find identifier for key: " + key);
            return false;
        }
        View subView = view.findViewById(id);
        if (subView == null) {
            _logger.error("Can not find view for key: " + key);
            return false;
        }
        processor.process((T) subView);
        return true;
    }

    @Override
    public void onError(Ad ad, AdError adError) {
        _logger.info("onError: " + adError.getErrorMessage());
        Utils.checkMainThread();
        _bridge.callCpp(kOnFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        if (_nativeAd == null || _nativeAd != ad)
            return;

        _nativeAd.unregisterView();

        processView(_nativeAdView, k__call_to_action, new ViewProcessor<Button>() {
            @Override
            public void process(Button view) {
                view.setVisibility(_nativeAd.hasCallToAction() ? View.VISIBLE : View.INVISIBLE);
                view.setText(_nativeAd.getAdCallToAction());
            }
        });

        processView(_nativeAdView, k__title, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                view.setText(_nativeAd.getAdvertiserName());
            }
        });

        processView(_nativeAdView, k__body, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                view.setText(_nativeAd.getAdBodyText());
            }
        });

        processView(_nativeAdView, k__social_context, new ViewProcessor<TextView>() {
            @Override
            public void process(TextView view) {
                view.setText(_nativeAd.getAdSocialContext());
            }
        });

        processView(_nativeAdView, k__ad_choices, new ViewProcessor<LinearLayout>() {
            @Override
            public void process(LinearLayout view) {
                // Remove old icons.
                view.removeAllViews();

                // Add the AdChoices icon.
                AdOptionsView adOptionsView = new AdOptionsView(_context, _nativeAd, null);
                view.addView(adOptionsView);
            }
        });

        processView(_nativeAdView, k__media, new ViewProcessor<MediaView>() {
            @Override
            public void process(MediaView mediaView) {
                final Button callToAction = _nativeAdView.findViewById(getIdentifier(k__call_to_action));
                final ImageView icon = _nativeAdView.findViewById(getIdentifier(k__icon));
                _nativeAd.registerViewForInteraction(_nativeAdView, mediaView, icon, Arrays.asList(callToAction, mediaView));
            }
        });

        _isAdLoaded = true;
        _bridge.callCpp(kOnLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
        _bridge.callCpp(kOnClicked());
    }

    @Override
    public void onLoggingImpression(Ad ad) {
        _logger.info("onLoggingImpression");
        Utils.checkMainThread();
    }

    @Override
    public void onMediaDownloaded(Ad ad) {
        _logger.info("onMediaDownloaded");
    }
}
