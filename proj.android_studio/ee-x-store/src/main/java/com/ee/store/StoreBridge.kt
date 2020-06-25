package com.ee.store

import android.app.Activity
import android.content.Context
import androidx.annotation.AnyThread
import com.android.billingclient.api.AcknowledgePurchaseParams
import com.android.billingclient.api.BillingClient
import com.android.billingclient.api.BillingClient.BillingResponseCode
import com.android.billingclient.api.BillingClient.SkuType
import com.android.billingclient.api.BillingClientStateListener
import com.android.billingclient.api.BillingFlowParams
import com.android.billingclient.api.BillingResult
import com.android.billingclient.api.ConsumeParams
import com.android.billingclient.api.Purchase
import com.android.billingclient.api.PurchaseHistoryRecord
import com.android.billingclient.api.SkuDetails
import com.android.billingclient.api.SkuDetailsParams
import com.ee.core.IMessageBridge
import com.ee.core.IPlugin
import com.ee.core.Logger
import com.ee.core.internal.deserialize
import com.ee.core.internal.serialize
import com.ee.core.registerAsyncHandler
import io.reactivex.rxjava3.android.schedulers.AndroidSchedulers
import io.reactivex.rxjava3.core.Completable
import io.reactivex.rxjava3.core.Observable
import io.reactivex.rxjava3.core.Single
import io.reactivex.rxjava3.disposables.CompositeDisposable
import io.reactivex.rxjava3.subjects.PublishSubject
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Polymorphic
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault

/**
 * Created by Zinge on 5/16/17.
 */
@ImplicitReflectionSerializer
@UnstableDefault
class StoreBridge(
    private val _bridge: IMessageBridge,
    private val _context: Context,
    private var _activity: Activity?) : IPlugin {
    companion object {
        private val _logger = Logger(StoreBridge::class.java.name)

        private const val kPrefix = "StoreBridge"
        private const val kConnect = "${kPrefix}Connect"
        private const val kGetSkuDetails = "${kPrefix}GetSkuDetails"
        private const val kGetPurchases = "${kPrefix}GetPurchases"
        private const val kGetPurchaseHistory = "${kPrefix}GetPurchaseHistory"
        private const val kPurchase = "${kPrefix}Purchase"
        private const val kConsume = "${kPrefix}Consume"
        private const val kAcknowledge = "${kPrefix}Acknowledge"
    }

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
    }

    @AnyThread
    private fun registerHandlers() {
        _bridge.registerAsyncHandler(kConnect) { _, resolver ->
            _disposable.add(connect().subscribe({
                @Serializable
                @Suppress("unused")
                class Response(
                    val result: Boolean
                )

                val response = Response(true)
                resolver.resolve(response.serialize())
            }) { exception ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val result: Boolean,
                    val message: String
                )

                val response = Response(false, exception.localizedMessage ?: "")
                resolver.resolve(response.serialize())
            })
        }
        _bridge.registerAsyncHandler(kGetSkuDetails) { message, resolver ->
            @Serializable
            class Request(
                val sku_type: String,
                val sku_list: List<String>
            )

            val request = deserialize<Request>(message)
            _disposable.add(getSkuDetails(request.sku_type, request.sku_list).subscribe({ detailsList ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val item: List<Map<String, @Polymorphic Any>>
                )

                val response = Response(true, detailsList.map(StoreUtils::convertSkuDetailsToDictionary))
                resolver.resolve(response.serialize())
            }) { exception ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val error: String
                )

                val response = Response(false, exception.localizedMessage ?: "")
                resolver.resolve(response.serialize())
            })
        }
        _bridge.registerAsyncHandler(kGetPurchases) { message, resolver ->
            _disposable.add(getPurchases(message).subscribe({ purchaseList ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val item: List<Map<String, @Polymorphic Any>>
                )

                val response = Response(true, purchaseList.map(StoreUtils::convertPurchaseToDictionary))
                resolver.resolve(response.serialize())
            }) { exception ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val error: String
                )

                val response = Response(false, exception.localizedMessage ?: "")
                resolver.resolve(response.serialize())
            })
        }
        _bridge.registerAsyncHandler(kGetPurchaseHistory) { message, resolver ->
            _disposable.add(getPurchaseHistory(message).subscribe({ recordList ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val item: List<Map<String, @Polymorphic Any>>
                )

                val response = Response(true, recordList.map(StoreUtils::convertRecordToDictionary))
                resolver.resolve(response.serialize())
            }) { exception ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val error: String
                )

                val response = Response(false, exception.localizedMessage ?: "")
                resolver.resolve(response.serialize())
            })
        }
        _bridge.registerAsyncHandler(kPurchase) { message, resolver ->
            _disposable.add(purchase(message).subscribe({ purchase ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val item: Map<String, @Polymorphic Any>
                )

                val response = Response(true, StoreUtils.convertPurchaseToDictionary(purchase))
                resolver.resolve(response.serialize())
            }) { exception ->
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean,
                    val error: String
                )

                val response = Response(false, exception.localizedMessage ?: "")
                resolver.resolve(response.serialize())
            })
        }
        _bridge.registerAsyncHandler(kConsume) { message, resolver ->
            _disposable.add(consume(message).subscribe({
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean
                )

                val response = Response(true)
                resolver.resolve(response.serialize())
            }) {
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean
                )

                val response = Response(false)
                resolver.resolve(response.serialize())
            })
        }
        _bridge.registerAsyncHandler(kAcknowledge) { message, resolver ->
            _disposable.add(acknowledge(message).subscribe({
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean
                )

                val response = Response(true)
                resolver.resolve(response.serialize())
            }) {
                @Serializable
                @Suppress("unused")
                class Response(
                    val successful: Boolean
                )

                val response = Response(false)
                resolver.resolve(response.serialize())
            })
        }
    }

    private fun deregisterHandlers() {
        _bridge.deregisterHandler(kConnect)
        _bridge.deregisterHandler(kGetSkuDetails)
        _bridge.deregisterHandler(kGetPurchases)
        _bridge.deregisterHandler(kGetPurchaseHistory)
        _bridge.deregisterHandler(kPurchase)
        _bridge.deregisterHandler(kConsume)
        _bridge.deregisterHandler(kAcknowledge)
    }

    fun connect(): Observable<BillingClient> {
        _connectObservable?.let { observable ->
            return@connect observable
        }
        val observable = Observable.create<BillingClient> { emitter ->
            val client = BillingClient
                .newBuilder(_context)
                .enablePendingPurchases()
                .setListener { result, purchases ->
                    _purchaseSubject.onNext(
                        PurchasesUpdate(result.responseCode, purchases ?: ArrayList()))
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

    fun getSkuDetails(@SkuType skuType: String,
                      skuList: List<String>): Single<List<SkuDetails>> {
        return getSkuDetails(SkuDetailsParams
            .newBuilder()
            .setSkusList(skuList)
            .setType(skuType)
            .build())
    }

    private fun getSkuDetails(params: SkuDetailsParams): Single<List<SkuDetails>> {
        return Single.create<List<SkuDetails>> { emitter ->
            emitter.setDisposable(connect().subscribe({ client ->
                client.querySkuDetailsAsync(params) { result, detailsList ->
                    if (emitter.isDisposed) {
                        // Fix UndeliverableException.
                        return@querySkuDetailsAsync
                    }
                    if (result.responseCode == BillingResponseCode.OK) {
                        // https://stackoverflow.com/questions/56832130/skudetailslist-returning-null
                        if (detailsList != null) {
                            for (details in detailsList) {
                                _skuDetailsList[details.sku] = details
                            }
                        }
                        emitter.onSuccess(detailsList ?: ArrayList())
                    } else {
                        emitter.onError(StoreException(result.responseCode))
                    }
                }
            }, emitter::onError))
        }.subscribeOn(_scheduler)
    }

    fun getPurchases(@SkuType skuType: String): Single<List<Purchase>> {
        return Single.create<List<Purchase>> { emitter ->
            emitter.setDisposable(connect().subscribe({ client ->
                val result = client.queryPurchases(skuType)
                if (result.responseCode == BillingResponseCode.OK) {
                    val purchaseList = result.purchasesList
                    emitter.onSuccess(purchaseList ?: ArrayList())
                } else {
                    emitter.onError(StoreException(result.responseCode))
                }
            }, emitter::onError))
        }.subscribeOn(_scheduler)
    }

    private fun getPurchaseHistory(@SkuType skuType: String): Single<List<PurchaseHistoryRecord>> {
        return Single.create<List<PurchaseHistoryRecord>> { emitter ->
            emitter.setDisposable(connect().subscribe({ client ->
                client.queryPurchaseHistoryAsync(skuType) { result, recordList ->
                    if (result.responseCode == BillingResponseCode.OK) {
                        emitter.onSuccess(recordList ?: ArrayList())
                    } else {
                        emitter.onError(StoreException(result.responseCode))
                    }
                }
            }, emitter::onError))
        }.subscribeOn(_scheduler)
    }

    fun purchase(sku: String): Single<Purchase> {
        return Single.create<Purchase> { emitter ->
            val details = _skuDetailsList[sku]
            if (details == null) {
                emitter.onError(IllegalStateException("Cannot find sku details"))
                return@create
            }
            emitter.setDisposable(purchase(details).subscribe(emitter::onSuccess, emitter::onError))
        }.subscribeOn(_scheduler)
    }

    private fun purchase(details: SkuDetails): Single<Purchase> {
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

    private fun launchBillingFlow(details: SkuDetails): Completable {
        return launchBillingFlow(BillingFlowParams
            .newBuilder()
            .setSkuDetails(details)
            .build())
    }

    private fun launchBillingFlow(params: BillingFlowParams): Completable {
        return Completable.create { emitter ->
            emitter.setDisposable(connect().subscribe { client ->
                if (_activity == null) {
                    emitter.onError(IllegalStateException("Activity is not available"))
                    return@subscribe
                }
                val result = client.launchBillingFlow(_activity, params)
                if (result.responseCode == BillingResponseCode.OK) {
                    emitter.onComplete()
                } else {
                    emitter.onError(StoreException(result.responseCode))
                }
            })
        }.subscribeOn(_scheduler)
    }

    fun consume(purchaseToken: String): Completable {
        return consume(ConsumeParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build())
    }

    private fun consume(params: ConsumeParams): Completable {
        return Completable.create { emitter ->
            emitter.setDisposable(connect().subscribe { client ->
                client.consumeAsync(params) { result, _ ->
                    if (result.responseCode == BillingResponseCode.OK) {
                        emitter.onComplete()
                    } else {
                        emitter.onError(StoreException(result.responseCode))
                    }
                }
            })
        }.subscribeOn(_scheduler)
    }

    fun acknowledge(purchaseToken: String): Completable {
        return acknowledge(AcknowledgePurchaseParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build())
    }

    private fun acknowledge(params: AcknowledgePurchaseParams): Completable {
        return Completable.create { emitter ->
            emitter.setDisposable(connect().subscribe { client ->
                client.acknowledgePurchase(params) { result ->
                    if (result.responseCode == BillingResponseCode.OK) {
                        emitter.onComplete()
                    } else {
                        emitter.onError(StoreException(result.responseCode))
                    }
                }
            })
        }.subscribeOn(_scheduler)
    }
}