package com.ee.store;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

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
import com.ee.core.IPlugin;
import com.ee.core.Logger;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Thread;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers;
import io.reactivex.rxjava3.core.Completable;
import io.reactivex.rxjava3.core.Observable;
import io.reactivex.rxjava3.core.Scheduler;
import io.reactivex.rxjava3.core.Single;
import io.reactivex.rxjava3.disposables.CompositeDisposable;
import io.reactivex.rxjava3.subjects.PublishSubject;
import java8.util.stream.StreamSupport;

import static com.google.common.truth.Truth.assertThat;

/**
 * Created by Zinge on 5/16/17.
 */

public class Store implements IPlugin {
    private static final Logger _logger = new Logger(Store.class.getName());

    private static final String kPrefix = "Store_";
    private static final String kConnect = kPrefix + "connect";
    private static final String kGetSkuDetails = kPrefix + "getSkuDetails";
    private static final String kGetPurchases = kPrefix + "getPurchases";
    private static final String kGetPurchaseHistory = kPrefix + "getPurchaseHistory";
    private static final String kPurchase = kPrefix + "purchase";
    private static final String kConsume = kPrefix + "consume";
    private static final String kAcknowledge = kPrefix + "acknowledge";

    private IMessageBridge _bridge;
    private Context _context;
    private Activity _activity;
    private Scheduler _scheduler;
    private CompositeDisposable _disposable;
    private PublishSubject<PurchasesUpdate> _purchaseSubject;
    private Observable<BillingClient> _connectObservable;
    private Map<String, SkuDetails> _skuDetailsList;

    public Store(@NonNull IMessageBridge bridge, @NonNull Context context, @Nullable Activity activity) {
        _bridge = bridge;
        _context = context;
        _activity = activity;
        _scheduler = AndroidSchedulers.mainThread();
        _disposable = new CompositeDisposable();
        _purchaseSubject = PublishSubject.create();
        _skuDetailsList = new HashMap<>();
        registerHandlers();
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
        Thread.checkMainThread();
        deregisterHandlers();
        _context = null;
        _bridge = null;
        _scheduler = null;
        _disposable.dispose();
        _disposable = null;
        _purchaseSubject = null;
        _connectObservable = null;
        _skuDetailsList = null;
    }

    private void registerHandlers() {
        _bridge.registerAsyncHandler(kConnect, (message, resolver) -> {
            _disposable.add(connect().subscribe(
                client -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("result", false);
                    result.put("message", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });

        _bridge.registerAsyncHandler(kGetSkuDetails, (message, resolver) -> {
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
                    result.put("successful", true);
                    result.put("item", list);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });

        _bridge.registerAsyncHandler(kGetPurchases, (message, resolver) -> {
            String skuType = message;
            _disposable.add(getPurchases(skuType).subscribe(
                purchaseList -> {
                    List<Object> list = new ArrayList<>();
                    for (Purchase purchase : purchaseList) {
                        list.add(StoreUtils.convertPurchaseToDictionary(purchase));
                    }

                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", true);
                    result.put("item", list);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });

        _bridge.registerAsyncHandler(kGetPurchaseHistory, (message, resolver) -> {
            String skuType = message;
            _disposable.add(getPurchaseHistory(skuType).subscribe(
                recordList -> {
                    List<Object> list = new ArrayList<>();
                    for (PurchaseHistoryRecord record : recordList) {
                        list.add(StoreUtils.convertRecordToDictionary(record));
                    }

                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", true);
                    result.put("item", list);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });

        _bridge.registerAsyncHandler(kPurchase, (message, resolver) -> {
            String sku = message;
            _disposable.add(purchase(sku).subscribe(
                purchase -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", true);
                    result.put("item", StoreUtils.convertPurchaseToDictionary(purchase));

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", false);
                    result.put("error", exception.getMessage());

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });

        _bridge.registerAsyncHandler(kConsume, (message, resolver) -> {
            String purchaseToken = message;
            _disposable.add(consume(purchaseToken).subscribe(
                () -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", false);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });

        _bridge.registerAsyncHandler(kAcknowledge, (message, resolver) -> {
            String purchaseToken = message;
            _disposable.add(acknowledge(purchaseToken).subscribe(
                () -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", true);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                },
                exception -> {
                    Map<String, Object> result = new HashMap<>();
                    result.put("successful", false);

                    String response = JsonUtils.convertDictionaryToString(result);
                    assertThat(response).isNotNull();
                    resolver.resolve(response);
                }));
        });
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

    public Observable<BillingClient> connect() {
        if (_connectObservable != null) {
            return _connectObservable;
        }
        Observable<BillingClient> observable = Observable.create(emitter -> {
            BillingClient client = BillingClient
                .newBuilder(_context)
                .enablePendingPurchases()
                .setListener((result, purchases) ->
                    _purchaseSubject.onNext(new PurchasesUpdate(result.getResponseCode(), purchases)))
                .build();
            client.startConnection(new BillingClientStateListener() {
                @Override
                public void onBillingSetupFinished(BillingResult result) {
                    if (emitter.isDisposed()) {
                        if (client.isReady()) {
                            client.endConnection();
                        }
                    } else {
                        if (result.getResponseCode() == BillingResponseCode.OK) {
                            _logger.info("Connected to BillingClient.");
                            emitter.onNext(client);
                        } else {
                            _logger.info("Could not connect to BillingClient. Response code = %s",
                                result.getResponseCode());
                            emitter.onError(new StoreException(result.getResponseCode()));
                        }
                    }
                }

                @Override
                public void onBillingServiceDisconnected() {
                    if (emitter.isDisposed()) {
                        // Fix UndeliverableException.
                    } else {
                        emitter.onComplete();
                    }
                }
            });
            emitter.setCancellable(() -> {
                if (client.isReady()) {
                    client.endConnection();
                }
            });
        });
        _connectObservable = observable
            .share()
            .repeat()
            .replay()
            .refCount()
            .subscribeOn(_scheduler);
        return _connectObservable;
    }

    public Single<List<SkuDetails>> getSkuDetails(@SkuType @NonNull String skuType,
                                                  @NonNull List<String> skuList) {
        return getSkuDetails(SkuDetailsParams
            .newBuilder()
            .setSkusList(skuList)
            .setType(skuType)
            .build());
    }

    private Single<List<SkuDetails>> getSkuDetails(@NonNull SkuDetailsParams params) {
        return Single.<List<SkuDetails>>create(emitter ->
            emitter.setDisposable(connect().subscribe(client ->
                client.querySkuDetailsAsync(params, (result, detailsList) -> {
                    if (emitter.isDisposed()) {
                        // Fix UndeliverableException.
                        return;
                    }
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        // https://stackoverflow.com/questions/56832130/skudetailslist-returning-null
                        if (detailsList != null) {
                            for (SkuDetails details : detailsList) {
                                String sku = details.getSku();
                                _skuDetailsList.remove(sku);
                                _skuDetailsList.put(sku, details);
                            }
                        }
                        emitter.onSuccess(detailsList != null ? detailsList : new ArrayList<>());
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }), emitter::onError)))
            .subscribeOn(_scheduler);
    }

    public Single<List<Purchase>> getPurchases(@SkuType @NonNull String skuType) {
        return Single.<List<Purchase>>create(emitter ->
            emitter.setDisposable(connect().subscribe(client -> {
                PurchasesResult result = client.queryPurchases(skuType);
                if (result.getResponseCode() == BillingResponseCode.OK) {
                    List<Purchase> purchaseList = result.getPurchasesList();
                    emitter.onSuccess(purchaseList != null ? purchaseList : new ArrayList<>());
                } else {
                    emitter.onError(new StoreException(result.getResponseCode()));
                }
            }, emitter::onError)))
            .subscribeOn(_scheduler);
    }

    private Single<List<PurchaseHistoryRecord>> getPurchaseHistory(@SkuType @NonNull String skuType) {
        return Single.<List<PurchaseHistoryRecord>>create(emitter ->
            emitter.setDisposable(connect().subscribe(client ->
                client.queryPurchaseHistoryAsync(skuType, (result, recordList) -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        emitter.onSuccess(recordList != null ? recordList : new ArrayList<>());
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }), emitter::onError)))
            .subscribeOn(_scheduler);
    }

    public Single<Purchase> purchase(@NonNull String sku) {
        return Single.<Purchase>create(emitter -> {
            SkuDetails details = _skuDetailsList.get(sku);
            if (details == null) {
                emitter.onError(new IllegalStateException("Cannot find sku details"));
                return;
            }
            emitter.setDisposable(
                purchase(details).subscribe(emitter::onSuccess, emitter::onError));
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
                    .subscribeOn(_scheduler)
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
                    }, emitter::onError))))
            .subscribeOn(_scheduler);
    }

    private Completable launchBillingFlow(@NonNull SkuDetails details) {
        return launchBillingFlow(BillingFlowParams
            .newBuilder()
            .setSkuDetails(details)
            .build());
    }

    private Completable launchBillingFlow(BillingFlowParams params) {
        return Completable.create(emitter ->
            emitter.setDisposable(connect().subscribe(client -> {
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
            })))
            .subscribeOn(_scheduler);
    }

    public Completable consume(@NonNull String purchaseToken) {
        return consume(ConsumeParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build());
    }

    private Completable consume(@NonNull ConsumeParams params) {
        return Completable.create(emitter ->
            emitter.setDisposable(connect().subscribe(client ->
                client.consumeAsync(params, (result, token) -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        emitter.onComplete();
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }))))
            .subscribeOn(_scheduler);
    }

    public Completable acknowledge(@NonNull String purchaseToken) {
        return acknowledge(AcknowledgePurchaseParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build());
    }

    private Completable acknowledge(@NonNull AcknowledgePurchaseParams params) {
        return Completable.create(emitter ->
            emitter.setDisposable(connect().subscribe(client ->
                client.acknowledgePurchase(params, result -> {
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        emitter.onComplete();
                    } else {
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }))))
            .subscribeOn(_scheduler);
    }
}
