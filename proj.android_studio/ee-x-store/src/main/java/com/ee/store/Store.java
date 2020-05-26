package com.ee.store;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;

import com.android.billingclient.api.AcknowledgePurchaseParams;
import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.BillingClient.SkuType;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.ConsumeParams;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.Purchase.PurchasesResult;
import com.android.billingclient.api.PurchaseHistoryRecord;
import com.android.billingclient.api.SkuDetails;
import com.android.billingclient.api.SkuDetailsParams;
import com.ee.core.IMessageBridge;
import com.ee.core.Logger;
import com.ee.core.MessageBridge;
import com.ee.core.PluginProtocol;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.reactivex.rxjava3.core.Completable;
import io.reactivex.rxjava3.core.Scheduler;
import io.reactivex.rxjava3.core.Single;
import io.reactivex.rxjava3.disposables.CompositeDisposable;
import io.reactivex.rxjava3.schedulers.Schedulers;
import io.reactivex.rxjava3.subjects.PublishSubject;
import java8.util.stream.StreamSupport;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 5/16/17.
 */

public class Store implements PluginProtocol {
    private static final Logger _logger = new Logger(Store.class.getName());

    private static final String kPrefix = "Store";
    private static final String kConnect = kPrefix + "_connect";
    private static final String kGetSkuDetails = kPrefix + "_getSkuDetails";
    private static final String kGetPurchases = kPrefix + "_getPurchases";
    private static final String kGetPurchaseHistory = kPrefix + "_getPurchaseHistory";
    private static final String kPurchase = kPrefix + "_getPurchase";
    private static final String kConsume = kPrefix + "_consume";
    private static final String kAcknowledge = kPrefix + "_acknowledge";

    private static final String kOnConnect = kPrefix + "_onConnect";
    private static final String kOnGetSkuDetails = kPrefix + "_onGetSkuDetails";
    private static final String kOnGetPurchases = kPrefix + "_onGetPurchases";
    private static final String kOnGetPurchaseHistory = kPrefix + "_onGetPurchaseHistory";
    private static final String kOnPurchase = kPrefix + "_onPurchase";
    private static final String kOnConsume = kPrefix + "_onConsume";
    private static final String kOnAcknowledge = kPrefix + "_onAcknowledge";

    private Context _context;
    private Activity _activity;
    private IMessageBridge _bridge;
    private Scheduler _scheduler;
    private CompositeDisposable _disposable;
    private PublishSubject<PurchasesUpdate> _purchaseSubject;
    private BillingClient _billingClient;
    private List<SkuDetails> _skuDetailsList;

    public Store(@NonNull Context context) {
        Utils.checkMainThread();
        _context = context;
        _bridge = MessageBridge.getInstance();
        _scheduler = Schedulers.io();
        _disposable = new CompositeDisposable();
        _purchaseSubject = PublishSubject.create();
        _skuDetailsList = new ArrayList<>();
        registerHandlers();
    }

    @NonNull
    @Override
    public String getPluginName() {
        return "Store";
    }

    @Override
    public void onCreate(@NonNull Activity activity) {
        _activity = activity;
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
        _activity = null;
    }

    @Override
    public void destroy() {
        Utils.checkMainThread();
        deregisterHandlers();
        _context = null;
        _bridge = null;
        _scheduler = null;
        _disposable.dispose();
        _disposable = null;
        _purchaseSubject = null;
        if (_billingClient != null) {
            _billingClient.endConnection();
            _billingClient = null;
        }
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
        _bridge.registerHandler(message -> {
            _disposable.add(connect().subscribe(
                client -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnConnect, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);
                    result.put("message", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnConnect, response);
                }));
            return "";
        }, kConnect);

        _bridge.registerHandler(message -> {
            Map<String, Object> dict = JsonUtils.convertStringToDictionary(message);
            assertThat(dict).isNotNull();

            String skuType = (String) dict.get("sku_type");
            List<String> skuList = (List<String>) dict.get("sku_list");
            assertThat(skuType).isNotNull();
            assertThat(skuList).isNotNull();

            _disposable.add(getSkuDetails(skuType, skuList).subscribe(
                detailsList -> {
                    List<Object> list = new ArrayList<>();
                    for (SkuDetails details : detailsList) {
                        list.add(StoreUtils.convertSkuDetailsToDictionary(details));
                    }

                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);
                    result.put("item", list);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnGetSkuDetails, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnGetSkuDetails, response);
                }));
            return "";
        }, kGetSkuDetails);

        _bridge.registerHandler(message -> {
            String skuType = message;
            _disposable.add(getPurchases(skuType).subscribe(
                purchaseList -> {
                    List<Object> list = new ArrayList<>();
                    for (Purchase purchase : purchaseList) {
                        list.add(StoreUtils.convertPurchaseToDictionary(purchase));
                    }

                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);
                    result.put("item", list);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnGetPurchases, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnGetPurchases, response);
                }));
            return "";
        }, kGetPurchases);

        _bridge.registerHandler(message -> {
            String skuType = message;
            _disposable.add(getPurchaseHistory(skuType).subscribe(
                recordList -> {
                    List<Object> list = new ArrayList<>();
                    for (PurchaseHistoryRecord record : recordList) {
                        list.add(StoreUtils.convertRecordToDictionary(record));
                    }

                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);
                    result.put("item", list);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnGetPurchaseHistory, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnGetPurchaseHistory, response);
                }));
            return "";
        }, kGetPurchaseHistory);

        _bridge.registerHandler(message -> {
            String sku = message;
            _disposable.add(purchase(sku).subscribe(
                purchase -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnPurchase, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnPurchase, response);
                }));
            return "";
        }, kPurchase);

        _bridge.registerHandler(message -> {
            String purchaseToken = message;
            _disposable.add(consume(purchaseToken).subscribe(
                () -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnConsume, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnConsume, response);
                }));
            return "";
        }, kConsume);

        _bridge.registerHandler(message -> {
            String purchaseToken = message;
            _disposable.add(acknowledge(purchaseToken).subscribe(
                () -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnAcknowledge, response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    _bridge.callCpp(kOnAcknowledge, response);
                }));
            return "";
        }, kAcknowledge);
    }

    private void deregisterHandlers() {
        _bridge.deregisterHandler(kConnect);
        _bridge.deregisterHandler(kGetSkuDetails);
        _bridge.deregisterHandler(kGetPurchases);
        _bridge.deregisterHandler(kGetPurchaseHistory);
        _bridge.deregisterHandler(kPurchase);
        _bridge.deregisterHandler(kConsume);
        _bridge.deregisterHandler(kAcknowledge);
    }

    private Single<BillingClient> connect() {
        return Single.<BillingClient>create(emitter -> {
            if (_billingClient != null && _billingClient.isReady()) {
                emitter.onSuccess(_billingClient);
                return;
            }
            BillingClient client = BillingClient
                .newBuilder(_context)
                .enablePendingPurchases()
                .setListener((result, purchases) ->
                    _purchaseSubject.onNext(new PurchasesUpdate(result.getResponseCode(), purchases)))
                .build();

            _billingClient = client;
            client.startConnection(new BillingClientStateListener() {
                @Override
                public void onBillingSetupFinished(BillingResult result) {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        _logger.info("Connected to BillingClient.");
                        emitter.onSuccess(client);
                    } else {
                        _logger.info("Could not connect to BillingClient. Response code = %s",
                            result.getResponseCode());
                        _billingClient = null;
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }

                @Override
                public void onBillingServiceDisconnected() {
                    _billingClient = null; // We'll build up a new connection upon next request.
                }
            });
        }).subscribeOn(_scheduler);
    }

    private Single<List<SkuDetails>> getSkuDetails(@SkuType @NonNull String skuType,
                                                   @NonNull List<String> skuList) {
        return getSkuDetails(SkuDetailsParams
            .newBuilder()
            .setSkusList(skuList)
            .setType(skuType)
            .build());
    }

    private Single<List<SkuDetails>> getSkuDetails(@NonNull SkuDetailsParams params) {
        return connect().flatMap(client ->
            Single.<List<SkuDetails>>create(emitter ->
                client.querySkuDetailsAsync(params, (result, detailsList) -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        // https://stackoverflow.com/questions/56832130/skudetailslist-returning-null
                        emitter.onSuccess(detailsList != null ? detailsList : new ArrayList<>());
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                })))
            .doOnSuccess(detailsList -> _skuDetailsList = detailsList)
            .subscribeOn(_scheduler);
    }

    private Single<List<Purchase>> getPurchases(@SkuType @NonNull String skuType) {
        return connect().flatMap(client ->
            Single.<List<Purchase>>create(emitter -> {
                PurchasesResult result = client.queryPurchases(skuType);
                if (result.getResponseCode() == BillingResponseCode.OK) {
                    List<Purchase> purchaseList = result.getPurchasesList();
                    emitter.onSuccess(purchaseList != null ? purchaseList : new ArrayList<>());
                } else {
                    emitter.onError(new StoreException(result.getResponseCode()));
                }
            })).subscribeOn(_scheduler);
    }

    private Single<List<PurchaseHistoryRecord>> getPurchaseHistory(@SkuType @NonNull String skuType) {
        return connect().flatMap(client ->
            Single.<List<PurchaseHistoryRecord>>create(emitter ->
                client.queryPurchaseHistoryAsync(skuType, (result, recordList) -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        emitter.onSuccess(recordList != null ? recordList : new ArrayList<>());
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }))).subscribeOn(_scheduler);
    }

    private Single<Purchase> purchase(@NonNull String sku) {
        return Single.<Purchase>create(emitter -> {
            SkuDetails details = StreamSupport
                .stream(_skuDetailsList)
                .filter(item -> item.getSku().equals(sku))
                .findFirst()
                .orElse(null);
            if (details == null) {
                emitter.onError(new IllegalStateException("Cannot find sku details"));
                return;
            }
            emitter.setDisposable(purchase(details).subscribe());
        }).subscribeOn(_scheduler);

    }

    private Single<Purchase> purchase(@NonNull SkuDetails details) {
        return launchBillingFlow(details).andThen(
            Single.<Purchase>create(emitter ->
                emitter.setDisposable(_purchaseSubject
                    .takeUntil(update -> {
                        if (update.purchases == null) {
                            return false;
                        }
                        return StreamSupport
                            .stream(update.purchases)
                            .anyMatch(item -> item.getSku().equals(details.getSku()));
                    })
                    .firstOrError()
                    .subscribe(update -> {
                        if (update.code == BillingResponseCode.OK) {
                            assertThat(update.purchases).isNotNull();
                            Purchase purchase = StreamSupport
                                .stream(update.purchases)
                                .filter(item -> item.getSku().equals(details.getSku()))
                                .findFirst()
                                .get();
                            emitter.onSuccess(purchase);
                        } else {
                            emitter.onError(new StoreException(update.code));
                        }
                    }, emitter::onError))
            )).subscribeOn(_scheduler);
    }

    private Completable launchBillingFlow(@NonNull SkuDetails details) {
        return launchBillingFlow(BillingFlowParams
            .newBuilder()
            .setSkuDetails(details)
            .build());
    }

    private Completable launchBillingFlow(BillingFlowParams params) {
        return connect().flatMapCompletable(client ->
            Completable.create(emitter -> {
                if (_activity == null) {
                    emitter.onError(new IllegalStateException("Activity is not available"));
                    return;
                }
                BillingResult result = client.launchBillingFlow(_activity, params);
                if (result.getResponseCode() == BillingResponseCode.OK) {
                    emitter.onComplete();
                } else {
                    emitter.onError(new StoreException(result.getResponseCode()));
                }
            })).subscribeOn(_scheduler);
    }

    private Completable consume(@NonNull String purchaseToken) {
        return consume(ConsumeParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build());
    }

    private Completable consume(@NonNull ConsumeParams params) {
        return connect().flatMapCompletable(client ->
            Completable.create(emitter ->
                client.consumeAsync(params, (result, token) -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        emitter.onComplete();
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }))).subscribeOn(_scheduler);
    }

    private Completable acknowledge(@NonNull String purchaseToken) {
        return acknowledge(AcknowledgePurchaseParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build());
    }

    private Completable acknowledge(@NonNull AcknowledgePurchaseParams params) {
        return connect().flatMapCompletable(client ->
            Completable.create(emitter -> {
                client.acknowledgePurchase(params, result -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        emitter.onComplete();
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                });
            })).subscribeOn(_scheduler);
    }
}
