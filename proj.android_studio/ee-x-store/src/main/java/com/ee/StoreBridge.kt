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
import com.android.billingclient.api.acknowledgePurchase
import com.android.billingclient.api.consumePurchase
import com.android.billingclient.api.queryPurchaseHistory
import com.android.billingclient.api.querySkuDetails
import com.ee.internal.GooglePlayPurchasing
import com.ee.internal.IUnityCallback
import com.ee.internal.IabHelper
import com.ee.internal.PurchasesUpdate
import com.ee.internal.StoreException
import com.ee.internal.UnityPurchasing
import com.ee.internal.deserialize
import com.ee.internal.serialize
import io.reactivex.rxjava3.core.Completable
import io.reactivex.rxjava3.core.Single
import kotlinx.coroutines.Deferred
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.async
import kotlinx.coroutines.cancel
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.launch
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlinx.serialization.InternalSerializationApi
import kotlinx.serialization.Serializable
import kotlin.coroutines.resume
import kotlin.coroutines.resumeWithException

/**
 * Created by Zinge on 5/16/17.
 */
@InternalSerializationApi
class StoreBridge(
    private val _bridge: IMessageBridge,
    private val _logger: ILogger,
    private val _context: Context,
    private var _activity: Activity?)
    : IPlugin, IStoreBridge, IUnityCallback {
    companion object {
        private val kTag = StoreBridge::class.java.name
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
    private val _purchasing = GooglePlayPurchasing(_logger, this, IabHelper(this), UnityPurchasing(this))
    private val _updateChannel = Channel<PurchasesUpdate>(Channel.UNLIMITED)
    private val _skuDetailsList: MutableMap<String, SkuDetails> = HashMap()
    private var _client: BillingClient? = null
    private var _clientAwaiter: Deferred<Unit>? = null

    init {
        _logger.info("$kTag: constructor begin: context = $_context")
        registerHandlers()
        _logger.info("$kTag: constructor end.")
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
        _purchasing.destroy()
        _client = null
        _clientAwaiter?.cancel()
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
        // Wait for the current connection to be completed.
        _clientAwaiter?.await()

        // Check if client exists.
        _client?.let {
            return@connect it
        }

        // Asynchronously create a connection.
        _clientAwaiter = _scope.async {
            // https://stackoverflow.com/questions/61217073/crash-in-android-billingclient-with-coroutines
            // https://stackoverflow.com/questions/61388646/billingclient-billingclientstatelistener-onbillingsetupfinished-is-called-multip
            suspendCancellableCoroutine<Unit> { cont ->
                val client = BillingClient
                    .newBuilder(_context)
                    .enablePendingPurchases()
                    .setListener { result, purchases ->
                        _scope.launch {
                            _updateChannel.send(
                                PurchasesUpdate(result.responseCode, purchases ?: ArrayList()))
                        }
                    }
                    .build()
                client.startConnection(object : BillingClientStateListener {
                    override fun onBillingSetupFinished(result: BillingResult) {
                        if (cont.isActive) {
                            if (result.responseCode == BillingResponseCode.OK) {
                                _logger.debug("$kTag: ${this::onBillingSetupFinished.name}: connected")
                                _client = client
                                cont.resume(Unit)
                            } else {
                                _logger.debug("$kTag: ${this::onBillingSetupFinished}: failed to connect, code = ${result.responseCode}")
                                cont.resumeWithException(StoreException(result.responseCode))
                            }
                        } else {
                            _logger.debug("$kTag: ${this::onBillingSetupFinished.name}: already resumed, code = ${result.responseCode}")
                        }
                    }

                    override fun onBillingServiceDisconnected() {
                        _logger.debug("$kTag: ${this::onBillingServiceDisconnected.name}")
                        _client = null
                    }
                })
            }
        }

        // Await.
        _clientAwaiter?.await()
        _clientAwaiter = null
        return _client ?: throw IllegalStateException("Client is not connected")
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
        val response = client.querySkuDetails(params)
        var result: List<SkuDetails> = ArrayList()
        if (response.billingResult.responseCode == BillingResponseCode.OK) {
            val detailsList = response.skuDetailsList
            if (detailsList != null) {
                for (details in detailsList) {
                    _skuDetailsList[details.sku] = details
                }
                result = detailsList
            }
        } else {
            throw StoreException(response.billingResult.responseCode)
        }
        return result
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

    override suspend fun getPurchaseHistory(@SkuType skuType: String): List<PurchaseHistoryRecord> {
        val client = connect()
        val response = client.queryPurchaseHistory(skuType)
        var result: List<PurchaseHistoryRecord> = ArrayList()
        if (response.billingResult.responseCode == BillingResponseCode.OK) {
            result = response.purchaseHistoryRecordList ?: result
        } else {
            throw StoreException(response.billingResult.responseCode)
        }
        return result
    }

    override suspend fun purchase(sku: String): Purchase {
        val details = _skuDetailsList[sku]
            ?: throw IllegalStateException("Cannot find sku details")
        return purchaseImpl(details)
    }

    private suspend fun purchaseImpl(details: SkuDetails): Purchase {
        launchBillingFlow(details)
        while (true) {
            val update = _updateChannel.receive()
            if (update.code != BillingResponseCode.OK) {
                throw StoreException(update.code)
            }
            if (update.purchases.all { it.sku != details.sku }) {
                continue
            }
            if (update.code == BillingResponseCode.OK) {
                return update.purchases.first { it.sku == details.sku }
            } else {
                throw StoreException(update.code)
            }
        }
    }

    private suspend fun launchBillingFlow(details: SkuDetails) {
        return launchBillingFlow(BillingFlowParams
            .newBuilder()
            .setSkuDetails(details)
            .build())
    }

    private suspend fun launchBillingFlow(params: BillingFlowParams) {
        val activity = _activity ?: throw IllegalStateException("Activity is not available")
        val client = connect()
        val result = client.launchBillingFlow(activity, params)
        if (result.responseCode == BillingResponseCode.OK) {
            // OK.
        } else {
            throw StoreException(result.responseCode)
        }
    }

    override suspend fun consume(purchaseToken: String) {
        return consume(ConsumeParams
            .newBuilder()
            .setPurchaseToken(purchaseToken)
            .build())
    }

    private suspend fun consume(params: ConsumeParams) {
        val client = connect()
        val response = client.consumePurchase(params)
        if (response.billingResult.responseCode == BillingResponseCode.OK) {
            // OK.
        } else {
            throw StoreException(response.billingResult.responseCode)
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
        val response = client.acknowledgePurchase(params)
        if (response.responseCode == BillingResponseCode.OK) {
            // OK.
        } else {
            throw StoreException(response.responseCode)
        }
    }

    override fun connectRx(): Single<BillingClient> {
        return Single.create { emitter ->
            _scope.launch {
                try {
                    val response = connect()
                    emitter.onSuccess(response)
                } catch (ex: Exception) {
                    emitter.onError(ex)
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