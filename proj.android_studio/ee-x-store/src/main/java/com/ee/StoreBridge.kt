package com.ee

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.android.billingclient.api.AcknowledgePurchaseParams
import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.BillingClient.BillingResponseCode
import com.android.billingclient.api.BillingClient.FeatureType
import com.android.billingclient.api.BillingClient.SkuType
import com.android.billingclient.api.BillingClientStateListener
import com.android.billingclient.api.BillingFlowParams
import com.android.billingclient.api.BillingResult
import com.android.billingclient.api.ConsumeParams
import com.android.billingclient.api.Purchase
import com.android.billingclient.api.PurchaseHistoryRecord
import com.android.billingclient.api.SkuDetails
import com.android.billingclient.api.SkuDetailsParams
import com.ee.internal.GooglePlayPurchasing
import com.ee.internal.IUnityCallback
import com.ee.internal.IabHelper
import com.ee.internal.PurchasesUpdate
import com.ee.internal.StoreException
import com.ee.internal.UnityPurchasing
import com.ee.internal.deserialize
import com.ee.internal.serialize
import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers
import io.reactivex.rxjava3.core.Completable
import io.reactivex.rxjava3.core.Observable
import io.reactivex.rxjava3.core.Single
import io.reactivex.rxjava3.disposables.CompositeDisposable
import io.reactivex.rxjava3.subjects.PublishSubject
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault
import kotlin.coroutines.resume
import kotlin.coroutines.resumeWithException
import kotlin.coroutines.suspendCoroutine

/**
 * Created by Zinge on 5/16/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
class StoreBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?)
    : IPlugin
    , IStoreBridge
    , IUnityCallback {
    companion object {
        private val _logger = Logger(StoreBridge::class.java.name)

        private const val kPrefix = "StoreBridge"
        private const val kRetrieveProducts = "${kPrefix}RetrieveProducts"
        private const val kPurchase = "${kPrefix}Purchase"
        private const val kFinishTransaction = "${kPrefix}FinishTransaction"
        private const val kOnSetupFailed = "${kPrefix}OnSetupFailed"
        private const val kOnProductsRetrieved = "${kPrefix}OnProductsRetrieved"
        private const val kOnPurchaseSucceeded = "${kPrefix}OnPurchaseSucceeded"
        private const val kOnPurchaseFailed = "${kPrefix}OnPurchaseFailed"
        private const val kGetProductJson = "${kPrefix}GetProductJson"
        private const val kRestoreTransactions = "${kPrefix}RestoreTransactions"
        private const val kFinishAdditionalTransaction = "${kPrefix}FinishAdditionalTransaction"
    }

    private val _scope = MainScope()
    private val _purchasing = GooglePlayPurchasing(this, IabHelper(this), UnityPurchasing(this))
    private val _scheduler = AndroidSchedulers.mainThread()
    private val _disposable = CompositeDisposable()
    private val _purchaseSubject = PublishSubject.create<PurchasesUpdate>()
    private var _connectObservable: Observable<BillingClient>? = null
    private val _skuDetailsList: MutableMap<String, SkuDetails> = HashMap()

    init {
        registerHandlers()
    }

    override fun onCreate(activity: Activity) {
        _activity = activity
    }

    override fun onStart() {}
    override fun onStop() {}
    override fun onResume() {}
    override fun onPause() {}
    override fun onDestroy() {
        _activity = null
    }

    override fun destroy() {
        deregisterHandlers()
        _disposable.dispose()
        _connectObservable = null
        _scope.cancel()
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerHandler(kRetrieveProducts) { message ->
            _purchasing.retrieveProducts(message)
            ""
        }
        _bridge.registerHandler(kPurchase) { message ->
            _purchasing.purchase(message)
            ""
        }
        _bridge.registerHandler(kFinishTransaction) { message ->
            @Serializable
            class Request(
                val productJson: String,
                val transactionId: String
            )

            val request = deserialize<Request>(message)
            _purchasing.finishTransaction(request.productJson, request.transactionId)
            ""
        }
        _bridge.registerHandler(kGetProductJson) {
            _purchasing.productJson
        }
        _bridge.registerAsyncHandler(kRestoreTransactions) {
            Utils.toString(_purchasing.restoreTransactions())
        }
        _bridge.registerHandler(kFinishAdditionalTransaction) { message ->
            @Serializable
            class Request(
                val productJson: String,
                val transactionId: String
            )

            val request = deserialize<Request>(message)
            _scope.launch {
                _purchasing.finishAdditionalTransaction(request.productJson, request.transactionId)
            }
            ""
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kRetrieveProducts)
        _bridge.deregisterHandler(kPurchase)
        _bridge.deregisterHandler(kFinishTransaction)
        _bridge.deregisterHandler(kGetProductJson)
        _bridge.deregisterHandler(kRestoreTransactions)
        _bridge.deregisterHandler(kFinishAdditionalTransaction)
    }

    override fun onSetupFailed(json: String) {
        _bridge.callCpp(kOnSetupFailed, json)
    }

    override fun onProductsRetrieved(json: String) {
        _bridge.callCpp(kOnProductsRetrieved, json)
    }

    override fun onPurchaseSucceeded(id: String, receipt: String, transactionId: String) {
        @Serializable
        @Suppress("unused")
        class Response(
            val id: String,
            val receipt: String,
            val transactionId: String
        )

        val response = Response(id, receipt, transactionId)
        _bridge.callCpp(kOnPurchaseSucceeded, response.serialize())
    }

    override fun onPurchaseFailed(json: String) {
        _bridge.callCpp(kOnPurchaseFailed, json)
    }

    override suspend fun connect(): BillingClient {
        return suspendCoroutine { cont ->
            _disposable.add(connectRx().subscribe({ client ->
                cont.resume(client)
            }) { exception ->
                cont.resumeWithException(exception)
            })
        }
    }

    override fun connectRx(): Observable<BillingClient> {
        _connectObservable?.let { observable ->
            return@connectRx observable
        }
        val observable = Observable.create<BillingClient> { emitter ->
            val client = BillingClient
                .newBuilder(_context)
                .enablePendingPurchases()
                .setListener { result, purchases ->
                    _purchaseSubject.onNext(
                        PurchasesUpdate(result.responseCode, purchases
                            ?: ArrayList()))
                }
                .build()
            client.startConnection(object : BillingClientStateListener {
                override fun onBillingSetupFinished(result: BillingResult) {
                    if (emitter.isDisposed) {
                        if (client.isReady) {
                            client.endConnection()
                        }
                    } else {
                        if (result.responseCode == BillingResponseCode.OK) {
                            _logger.info("Connected to BillingClient.")
                            emitter.onNext(client)
                        } else {
                            _logger.info("Could not connect to BillingClient. Response code = ${result.responseCode}")
                            emitter.onError(StoreException(result.responseCode))
                        }
                    }
                }

                override fun onBillingServiceDisconnected() {
                    if (emitter.isDisposed) {
                        // Fix UndeliverableException.
                    } else {
                        emitter.onComplete()
                    }
                }
            })
            emitter.setCancellable {
                if (client.isReady) {
                    client.endConnection()
                }
            }
        }
        _connectObservable = observable
            .share()
            .repeat()
            .replay()
            .refCount()
            .subscribeOn(_scheduler)
        return observable
    }

    override suspend fun isFeatureSupported(@FeatureType featureType: String): Boolean {
        val client = connect()
        val result = client.isFeatureSupported(featureType)
        return result.responseCode == BillingResponseCode.OK
    }

    override suspend fun getSkuDetails(@SkuType skuType: String,
                                       skuList: List<String>): List<SkuDetails> {
        return getSkuDetails(SkuDetailsParams
            .newBuilder()
            .setSkusList(skuList)
            .setType(skuType)
            .build())
    }

    private suspend fun getSkuDetails(params: SkuDetailsParams): List<SkuDetails> {
        val client = connect()
        return suspendCoroutine { cont ->
            client.querySkuDetailsAsync(params) { result, detailsList ->
                if (result.responseCode == BillingResponseCode.OK) {
                    // https://stackoverflow.com/questions/56832130/skudetailslist-returning-null
                    if (detailsList != null) {
                        for (details in detailsList) {
                            _skuDetailsList[details.sku] = details
                        }
                    }
                    cont.resume(detailsList ?: ArrayList())
                } else {
                    cont.resumeWithException(StoreException(result.responseCode))
                }
            }
        }
    }

    override fun getSkuDetailsRx(skuType: String, skuList: List<String>): Single<List<SkuDetails>> {
        return Single.create { emitter ->
            _scope.launch {
                try {
                    val response = getSkuDetails(skuType, skuList)
                    emitter.onSuccess(response)
                } catch (ex: Exception) {
                    emitter.onError(ex)
                }
            }
        }
    }

    override suspend fun getPurchases(@SkuType skuType: String): List<Purchase> {
        val client = connect()
        val result = client.queryPurchases(skuType)
        if (result.responseCode == BillingResponseCode.OK) {
            val purchaseList = result.purchasesList
            return purchaseList ?: ArrayList()
        } else {
            throw StoreException(result.responseCode)
        }
    }

    override fun getPurchasesRx(skuType: String): Single<List<Purchase>> {
        return Single.create { emitter ->
            _scope.launch {
                try {
                    val response = getPurchases(skuType)
                    emitter.onSuccess(response)
                } catch (ex: Exception) {
                    emitter.onError(ex)
                }
            }
        }
    }

    override suspend fun getPurchaseHistory(@SkuType skuType: String): List<PurchaseHistoryRecord> {
        val client = connect()
        return suspendCoroutine { cont ->
            client.queryPurchaseHistoryAsync(skuType) { result, recordList ->
                if (result.responseCode == BillingResponseCode.OK) {
                    cont.resume(recordList ?: ArrayList())
                } else {
                    cont.resumeWithException(StoreException(result.responseCode))
                }
            }
        }
    }

    override fun getPurchaseHistoryRx(skuType: String): Single<List<PurchaseHistoryRecord>> {
        return Single.create { emitter ->
            _scope.launch {
                try {
                    val response = getPurchaseHistory(skuType)
                    emitter.onSuccess(response)
                } catch (ex: Exception) {
                    emitter.onError(ex)
                }
            }
        }
    }

    override suspend fun purchase(sku: String): Purchase {
        val details = _skuDetailsList[sku]
            ?: throw IllegalStateException("Cannot find sku details")
        return suspendCoroutine { cont ->
            _disposable.add(purchaseImpl(details).subscribe({ purchase ->
                cont.resume(purchase)
            }) { exception ->
                cont.resumeWithException(exception)
            })
        }
    }

    private fun purchaseImpl(details: SkuDetails): Single<Purchase> {
        return launchBillingFlow(details).andThen(
            Single.create<Purchase> { emitter ->
                emitter.setDisposable(_purchaseSubject
                    .takeUntil { update ->
                        update.purchases.any { item ->
                            item.sku == details.sku
                        }
                    }
                    .firstOrError()
                    .subscribeOn(_scheduler)
                    .subscribe({ update: PurchasesUpdate ->
                        if (update.code == BillingResponseCode.OK) {
                            val purchase = update.purchases.first { item ->
                                item.sku == details.sku
                            }
                            emitter.onSuccess(purchase)
                        } else {
                            emitter.onError(StoreException(update.code))
                        }
                    }, emitter::onError))
            }).subscribeOn(_scheduler)
    }

    override fun purchaseRx(sku: String): Single<Purchase> {
        return Single.create { emitter ->
            _scope.launch {
                try {
                    val response = purchase(sku)
                    emitter.onSuccess(response)
                } catch (ex: Exception) {
                    emitter.onError(ex)
                }
            }
        }
    }

    private fun launchBillingFlow(details: SkuDetails): Completable {
        return launchBillingFlow(BillingFlowParams
            .newBuilder()
            .setSkuDetails(details)
            .build())
    }

    private fun launchBillingFlow(params: BillingFlowParams): Completable {
        return Completable.create { emitter ->
            emitter.setDisposable(connectRx().subscribe { client ->
                val activity = _activity
                if (activity == null) {
                    emitter.onError(IllegalStateException("Activity is not available"))
                    return@subscribe
                }
                val result = client.launchBillingFlow(activity, params)
                if (result.responseCode == BillingResponseCode.OK) {
                    emitter.onComplete()
                } else {
                    emitter.onError(StoreException(result.responseCode))
                }
            })
        }.subscribeOn(_scheduler)
    }

    override suspend fun consume(purchaseToken: String) {
        return consume(ConsumeParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build())
    }

    private suspend fun consume(params: ConsumeParams) {
        val client = connect()
        return suspendCoroutine { cont ->
            client.consumeAsync(params) { result, _ ->
                if (result.responseCode == BillingResponseCode.OK) {
                    cont.resume(Unit)
                } else {
                    cont.resumeWithException(StoreException(result.responseCode))
                }
            }
        }
    }

    override fun consumeRx(purchaseToken: String): Completable {
        return Completable.create { emitter ->
            _scope.launch {
                try {
                    consume(purchaseToken)
                    emitter.onComplete()
                } catch (ex: Exception) {
                    emitter.onError(ex)
                }
            }
        }
    }

    override suspend fun acknowledge(purchaseToken: String) {
        return acknowledge(AcknowledgePurchaseParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build())
    }

    private suspend fun acknowledge(params: AcknowledgePurchaseParams) {
        val client = connect()
        return suspendCoroutine { cont ->
            client.acknowledgePurchase(params) { result ->
                if (result.responseCode == BillingResponseCode.OK) {
                    cont.resume(Unit)
                } else {
                    cont.resumeWithException(StoreException(result.responseCode))
                }
            }
        }
    }

    override fun acknowledgeRx(purchaseToken: String): Completable {
        return Completable.create { emitter ->
            _scope.launch {
                try {
                    acknowledge(purchaseToken)
                    emitter.onComplete()
                } catch (ex: Exception) {
                    emitter.onError(ex)
                }
            }
        }
    }
}