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
import com.ee.ads.MessageHelper;
import com.ee.ads.ViewHelper;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.internal.Utils;
import com.facebook.ads.Ad;
import com.facebook.ads.AdError;
import com.facebook.ads.AdOptionsView;
import com.facebook.ads.MediaView;
import com.facebook.ads.NativeAd;
import com.facebook.ads.NativeAdListener;

import java.util.Arrays;
import java.util.Map;

import static com.google.common.truth.Truth.assertThat;

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

    private static final Logger _logger = new Logger(FacebookNativeAd.class.getName());

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
    private NativeAd _ad;
    private View _view;

    FacebookNativeAd(@NonNull Context context, @Nullable Activity activity,
                     @NonNull String adId, @NonNull String layoutName, @NonNull Map<String, String> identifiers) {
        _logger.info("constructor: adId = %s", adId);
        Utils.checkMainThread();
        _context = context;
        _activity = activity;
        _bridge = MessageBridge.getInstance();
        _adId = adId;
        _layoutName = layoutName;
        _identifiers = identifiers;
        _messageHelper = new MessageHelper("FacebookNativeAd", adId);
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

    @SuppressWarnings("WeakerAccess")
    public void destroy() {
        _logger.info("destroy: adId = " + _adId);
        Utils.checkMainThread();
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

        _bridge.registerHandler(message ->
            Utils.toString(createInternalAd()), _messageHelper.createInternalAd());

        _bridge.registerHandler(message ->
            Utils.toString(destroyInternalAd()), _messageHelper.destroyInternalAd());
    }

    private void deregisterHandlers() {
        _helper.deregisterHandlers();

        _bridge.deregisterHandler(_messageHelper.createInternalAd());
        _bridge.deregisterHandler(_messageHelper.destroyInternalAd());
    }

    private boolean createInternalAd() {
        Utils.checkMainThread();
        if (_ad != null) {
            return false;
        }
        _isLoaded = false;
        _ad = new NativeAd(_context, _adId);
        return true;
    }

    private boolean destroyInternalAd() {
        Utils.checkMainThread();
        if (_ad == null) {
            return false;
        }
        _isLoaded = false;
        _ad.unregisterView();
        _ad.destroy();
        _ad = null;
        return true;
    }

    private void createView() {
        Utils.checkMainThread();
        int layoutId = _context.getResources()
            .getIdentifier(_layoutName, "layout", _context.getPackageName());
        View nativeAdView = LayoutInflater
            .from(_context)
            .inflate(layoutId, null, false);
        nativeAdView.setVisibility(View.INVISIBLE);

        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.START | Gravity.TOP;
        nativeAdView.setLayoutParams(params);

        _view = nativeAdView;
        _viewHelper = new ViewHelper(_view);

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
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        return _isLoaded;
    }

    @Override
    public void load() {
        Utils.checkMainThread();
        assertThat(_ad).isNotNull();
        _logger.info("load");

        ///
        /// Audience Network supports pre-caching video or image assets which enables
        /// the MediaView to play videos immediately after nativeAd finishes loading.
        /// https://developers.facebook.com/docs/audience-network/android-native#mediaview
        ///
        _ad.loadAd(_ad.buildLoadAdConfig()
            .withAdListener(this)
            .withMediaCacheFlag(NativeAd.MediaCacheFlag.ALL)
            .build());
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
        _bridge.callCpp(_messageHelper.onFailedToLoad(), adError.getErrorMessage());
    }

    @Override
    public void onAdLoaded(Ad ad) {
        _logger.info("onAdLoaded");
        Utils.checkMainThread();
        assertThat(_ad == ad);

        _ad.unregisterView();

        processView(_view, k__call_to_action, (ViewProcessor<Button>) view -> {
            view.setVisibility(_ad.hasCallToAction() ? View.VISIBLE : View.INVISIBLE);
            view.setText(_ad.getAdCallToAction());
        });

        processView(_view, k__title, (ViewProcessor<TextView>) view ->
            view.setText(_ad.getAdvertiserName()));

        processView(_view, k__body, (ViewProcessor<TextView>) view ->
            view.setText(_ad.getAdBodyText()));

        processView(_view, k__social_context, (ViewProcessor<TextView>) view ->
            view.setText(_ad.getAdSocialContext()));

        processView(_view, k__ad_choices, (ViewProcessor<LinearLayout>) view -> {
            // Remove old icons.
            view.removeAllViews();

            // Add the AdChoices icon.
            AdOptionsView adOptionsView = new AdOptionsView(_context, _ad, null);
            view.addView(adOptionsView);
        });

        processView(_view, k__media, (ViewProcessor<MediaView>) mediaView -> {
            final Button callToAction = _view.findViewById(getIdentifier(k__call_to_action));
            final ImageView icon = _view.findViewById(getIdentifier(k__icon));
            _ad.registerViewForInteraction(_view, mediaView, icon, Arrays.asList(callToAction, mediaView));
        });

        _isLoaded = true;
        _bridge.callCpp(_messageHelper.onLoaded());
    }

    @Override
    public void onAdClicked(Ad ad) {
        _logger.info("onAdClicked");
        Utils.checkMainThread();
        _bridge.callCpp(_messageHelper.onClicked());
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
