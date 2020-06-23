package com.ee.google.analytics;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.ee.core.IMessageBridge;
import com.ee.core.IPlugin;
import com.ee.core.Logger;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Thread;
import com.ee.core.internal.Utils;
import com.google.android.gms.analytics.HitBuilders;
import com.google.android.gms.analytics.ecommerce.Product;
import com.google.android.gms.analytics.ecommerce.ProductAction;

import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Set;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 10/13/17.
 */
public class GoogleAnalytics implements IPlugin {
    private static final String k__setDispatchInterval =
        "GoogleAnalytics_setDispatchInterval";
    private static final String k__setDryRun = "GoogleAnalytics_setDryRun";
    private static final String k__setOptOut = "GoogleAnalytics_setOptOut";
    private static final String k__setTrackUncaughtException =
        "GoogleAnalytics_setTrackUncaughtException";
    private static final String k__dispatch = "GoogleAnalytics_dispatch";
    private static final String k__createTracker = "GoogleAnalytics_createTracker";
    private static final String k__destroyTracker = "GoogleAnalytics_destroyTracker";

    private static final String k__testTrackEvent = "GoogleAnalytics_testTrackEvent";
    private static final String k__testTrackException =
        "GoogleAnalytics_testTrackException";
    private static final String k__testTrackScreenView =
        "GoogleAnalytics_testTrackScreenView";
    private static final String k__testTrackSocial = "GoogleAnalytics_testTrackSocial";
    private static final String k__testTrackTiming = "GoogleAnalytics_testTrackTiming";
    private static final String k__testCustomDimensionAndMetric =
        "GoogleAnalytics_testCustomDimensionAndMetric";
    private static final String k__testTrackEcommerceAction =
        "GoogleAnalytics_testTrackEcommerceAction";
    private static final String k__testTrackEcommerceImpression =
        "GoogleAnalytics_testTrackEcommerceImpression";

    private static final Logger _logger = new Logger(GoogleAnalytics.class.getName());

    private IMessageBridge _bridge;
    private Context _context;
    private com.google.android.gms.analytics.GoogleAnalytics _analytics;
    private Map<String, GoogleAnalyticsTracker> _trackers;
    private boolean _exceptionReportingEnabled;

    public GoogleAnalytics(@NonNull IMessageBridge bridge, @NonNull Context context, @Nullable Activity activity) {
        Thread.checkMainThread();
        _bridge = bridge;
        _context = context;
        _analytics = com.google.android.gms.analytics.GoogleAnalytics.getInstance(_context);
        _trackers = new HashMap<>();
        _exceptionReportingEnabled = false;
        registerHandlers();
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
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
    }

    @Override
    public void destroy() {
        Thread.checkMainThread();
        deregisterHandlers();
        for (String key : _trackers.keySet()) {
            GoogleAnalyticsTracker tracker = _trackers.get(key);
            tracker.destroy();
        }
        _trackers.clear();
        _trackers = null;
        _analytics = null;
        _context = null;
        _bridge = null;
    }

    private void registerHandlers() {
        _bridge.registerHandler(k__setDispatchInterval, message -> {
            int seconds = Integer.valueOf(message);
            setLocalDispatchInterval(seconds);
            return "";
        });

        _bridge.registerHandler(k__setDryRun, message -> {
            setDryRun(Utils.toBoolean(message));
            return "";
        });

        _bridge.registerHandler(k__setOptOut, message -> {
            setAppOptOut(Utils.toBoolean(message));
            return "";
        });

        _bridge.registerHandler(k__setTrackUncaughtException, message -> {
            setExceptionReportingEnabled(Utils.toBoolean(message));
            return "";
        });

        _bridge.registerHandler(k__dispatch, message -> {
            dispatchLocalHits();
            return "";
        });

        _bridge.registerHandler(k__createTracker, message -> {
            String trackingId = message;
            return Utils.toString(createTracker(trackingId));
        });

        _bridge.registerHandler(k__destroyTracker, message -> {
            String trackingId = message;
            return Utils.toString(destroyTracker(trackingId));
        });

        _bridge.registerHandler(k__testTrackEvent, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackEvent(dict));
        });

        _bridge.registerHandler(k__testTrackException, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackException(dict));
        });

        _bridge.registerHandler(k__testTrackScreenView, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackScreenView(dict));
        });

        _bridge.registerHandler(k__testTrackSocial, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackSocial(dict));
        });

        _bridge.registerHandler(k__testTrackTiming, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackTiming(dict));
        });

        _bridge.registerHandler(k__testCustomDimensionAndMetric, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testCustomDimensionAndMetric(dict));
        });

        _bridge.registerHandler(k__testTrackEcommerceAction, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackEcommerceAction(dict));
        });

        _bridge.registerHandler(k__testTrackEcommerceImpression, message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();
            return Utils.toString(testTrackEcommerceImpression(dict));
        });
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(k__setDispatchInterval);
        _bridge.deregisterHandler(k__setDryRun);
        _bridge.deregisterHandler(k__setOptOut);
        _bridge.deregisterHandler(k__setTrackUncaughtException);
        _bridge.deregisterHandler(k__dispatch);
        _bridge.deregisterHandler(k__createTracker);
        _bridge.deregisterHandler(k__destroyTracker);

        _bridge.deregisterHandler(k__testTrackEvent);
        _bridge.deregisterHandler(k__testTrackException);
        _bridge.deregisterHandler(k__testTrackScreenView);
        _bridge.deregisterHandler(k__testTrackSocial);
        _bridge.deregisterHandler(k__testTrackTiming);
        _bridge.deregisterHandler(k__testCustomDimensionAndMetric);
        _bridge.deregisterHandler(k__testTrackEcommerceAction);
        _bridge.deregisterHandler(k__testTrackEcommerceImpression);
    }

    @SuppressWarnings("WeakerAccess")
    public void setLocalDispatchInterval(int interval) {
        _analytics.setLocalDispatchPeriod(interval);
    }

    @SuppressWarnings("WeakerAccess")
    public void setDryRun(boolean enabled) {
        _analytics.setDryRun(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public void setAppOptOut(boolean enabled) {
        _analytics.setAppOptOut(enabled);
    }

    @SuppressWarnings("WeakerAccess")
    public void setExceptionReportingEnabled(boolean enabled) {
        _exceptionReportingEnabled = enabled;
        for (String key : _trackers.keySet()) {
            GoogleAnalyticsTracker tracker = _trackers.get(key);
            tracker.setExceptionReportingEnabled(enabled);
        }
    }

    @SuppressWarnings("WeakerAccess")
    public void dispatchLocalHits() {
        _analytics.dispatchLocalHits();
    }

    @SuppressWarnings("WeakerAccess")
    public boolean createTracker(@NonNull String trackingId) {
        if (_trackers.containsKey(trackingId)) {
            return false;
        }
        GoogleAnalyticsTracker tracker =
            new GoogleAnalyticsTracker(_bridge, _analytics, trackingId);
        tracker.setExceptionReportingEnabled(_exceptionReportingEnabled);
        _trackers.put(trackingId, tracker);
        return true;
    }

    @SuppressWarnings("WeakerAccess")
    public boolean destroyTracker(@NonNull String trackingId) {
        if (!_trackers.containsKey(trackingId)) {
            return false;
        }
        GoogleAnalyticsTracker tracker = _trackers.get(trackingId);
        tracker.destroy();
        _trackers.remove(trackingId);
        return true;
    }

    @Nullable
    public GoogleAnalyticsTracker getTracker(@NonNull String trackingId) {
        if (!_trackers.containsKey(trackingId)) {
            return null;
        }
        return _trackers.get(trackingId);
    }

    private boolean checkDictionary(@NonNull Map<String, Object> builtDict,
                                    @NonNull Map<String, String> expectedDict) {
        if (builtDict.size() != expectedDict.size()) {
            _logger.error(String.format(Locale.getDefault(),
                "Dictionary size mismatched: expected %d found %d", expectedDict.size(),
                builtDict.size()));
            return false;
        }
        Set<String> allKeys = expectedDict.keySet();
        boolean matched = true;
        for (String key : allKeys) {
            String expectedValue = expectedDict.get(key);
            String value = (String) builtDict.get(key);
            if (!expectedValue.equals(value)) {
                _logger.error(String.format(Locale.getDefault(),
                    "Element value mismatched: expected %s found %s", expectedValue, value));
                matched = false;
            }
        }
        return matched;
    }

    private boolean testTrackEvent(@NonNull Map<String, Object> dict) {
        Map<String, String> expectedDict = new HitBuilders.EventBuilder()
            .setCategory("category")
            .setAction("action")
            .setLabel("label")
            .setValue(1)
            .build();
        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean testTrackException(@NonNull Map<String, Object> dict) {
        Map<String, String> expectedDict =
            new HitBuilders.ExceptionBuilder().setDescription("description").setFatal(true).build();
        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean testTrackScreenView(@NonNull Map<String, Object> dict) {
        Map<String, String> expectedDict = new HitBuilders.ScreenViewBuilder().build();
        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean testTrackSocial(@NonNull Map<String, Object> dict) {
        Map<String, String> expectedDict = new HitBuilders.SocialBuilder()
            .setNetwork("network")
            .setAction("action")
            .setTarget("target")
            .build();
        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    private boolean testTrackTiming(@NonNull Map<String, Object> dict) {
        Map<String, String> expectedDict = new HitBuilders.TimingBuilder()
            .setCategory("category")
            .setValue(1)
            .setVariable("variable")
            .setLabel("label")
            .build();
        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean testCustomDimensionAndMetric(@NonNull Map<String, Object> dict) {
        Map<String, String> expectedDict = new HitBuilders.ScreenViewBuilder()
            .setCustomMetric(1, 1)
            .setCustomMetric(2, 2)
            .setCustomMetric(5, 5.5f)
            .setCustomDimension(1, "dimension_1")
            .setCustomDimension(2, "dimension_2")
            .build();
        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean testTrackEcommerceAction(@NonNull Map<String, Object> dict) {
        Product product0 =
            new Product().setCategory("category0").setId("id0").setName("name0").setPrice(1.5);
        Product product1 =
            new Product().setCategory("category1").setId("id1").setName("name1").setPrice(2);
        ProductAction action = new ProductAction(ProductAction.ACTION_PURCHASE)
            .setProductActionList("actionList")
            .setProductListSource("listSource")
            .setTransactionId("transactionId")
            .setTransactionRevenue(2.0);
        Map<String, String> expectedDict = new HitBuilders.ScreenViewBuilder()
            .addProduct(product0)
            .addProduct(product1)
            .setProductAction(action)
            .build();

        return checkDictionary(dict, expectedDict);
    }

    @SuppressWarnings("WeakerAccess")
    public boolean testTrackEcommerceImpression(@NonNull Map<String, Object> dict) {
        Product product0 =
            new Product().setCategory("category0").setId("id0").setName("name0").setPrice(1.5);
        Product product1 =
            new Product().setCategory("category1").setId("id1").setName("name1").setPrice(2.5);
        Product product2 =
            new Product().setCategory("category2").setId("id2").setName("name2").setPrice(3.0);
        Product product3 =
            new Product().setCategory("category3").setId("id3").setName("name3").setPrice(4);
        Map<String, String> expectedDict = new HitBuilders.ScreenViewBuilder()
            .addImpression(product0, "impressionList0")
            .addImpression(product1, "impressionList0")
            .addImpression(product2, "impressionList1")
            .addImpression(product3, "impressionList1")
            .build();

        return checkDictionary(dict, expectedDict);
    }
}
