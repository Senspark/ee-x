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
import com.ee.core.IPlugin;
import com.ee.core.internal.JsonUtils;
import com.ee.core.internal.Utils;

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
    private Observable<BillingClient> _connectObservable;
    private Map<String, SkuDetails> _skuDetailsList;

    public Store(@NonNull Context context, @NonNull IMessageBridge bridge) {
        Utils.checkMainThread();
        _context = context;
        _bridge = bridge;
        _scheduler = AndroidSchedulers.mainThread();
        _disposable = new CompositeDisposable();
        _purchaseSubject = PublishSubject.create();
        _skuDetailsList = new HashMap<>();
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
        _connectObservable = null;
        _skuDetailsList = null;
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
                    if (result.getResponseCode() == BillingResponseCode.OK) {
                        _logger.info("Connected to BillingClient.");
                        emitter.onNext(client);
                    } else {
                        _logger.info("Could not connect to BillingClient. Response code = %s",
                            result.getResponseCode());
                        emitter.onError(new StoreException(result.getResponseCode()));
                    }
                }

                @Override
                public void onBillingServiceDisconnected() {
                    emitter.onComplete();
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
