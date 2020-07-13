package com.ee.internal

import android.os.RemoteException
import com.android.billingclient.api.BillingClient.BillingResponseCode
import com.android.billingclient.api.BillingClient.SkuType
import com.android.billingclient.api.Purchase
import com.android.billingclient.api.SkuDetails
import com.ee.IStoreBridge
import com.ee.Logger
import kotlinx.coroutines.MainScope
import kotlinx.coroutines.cancel
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.UnstableDefault
import kotlinx.serialization.json.JsonException
import kotlin.math.min

@ImplicitReflectionSerializer
@UnstableDefault
class GooglePlayPurchasing(
    private val _plugin: IStoreBridge,
    private val _helper: IabHelper,
    private val _unityPurchasing: IStoreCallback)
    : StoreDeserializer() {
    companion object {
        private val _logger = Logger(GooglePlayPurchasing::class.java.name)
        private val _billingResponseCodeNames = mapOf(
            0 to "BILLING_RESPONSE_RESULT_OK",
            1 to "BILLING_RESPONSE_RESULT_USER_CANCELED",
            2 to "BILLING_RESPONSE_RESULT_SERVICE_UNAVAILABLE",
            3 to "BILLING_RESPONSE_RESULT_BILLING_UNAVAILABLE",
            4 to "BILLING_RESPONSE_RESULT_ITEM_UNAVAILABLE",
            5 to "BILLING_RESPONSE_RESULT_DEVELOPER_ERROR",
            6 to "BILLING_RESPONSE_RESULT_ERROR",
            7 to "BILLING_RESPONSE_RESULT_ITEM_ALREADY_OWNED",
            8 to "BILLING_RESPONSE_RESULT_ITEM_NOT_OWNED",
            -1000 to "IABHELPER_ERROR_BASE",
            -1001 to "IABHELPER_REMOTE_EXCEPTION",
            -1002 to "IABHELPER_BAD_RESPONSE",
            -1003 to "IABHELPER_VERIFICATION_FAILED",
            -1004 to "IABHELPER_SEND_INTENT_FAILED",
            -1005 to "IABHELPER_USER_CANCELLED",
            -1006 to "IABHELPER_UNKNOWN_PURCHASE_RESPONSE",
            -1007 to "IABHELPER_MISSING_TOKEN",
            -1008 to "IABHELPER_UNKNOWN_ERROR",
            -1009 to "IABHELPER_SUBSCRIPTIONS_NOT_AVAILABLE",
            -1010 to "IABHELPER_INVALID_CONSUMPTION"
        )
    }

    private val _scope = MainScope()
    private var _inventory = Inventory()
    private val _suspectFailedConsumableSkus: MutableSet<String> = HashSet()
    private var _productJson: String? = null
    private var _offlineBackOffTime = 5000L
    private var _purchaseInProgress = false

    fun destroy() {
        _scope.cancel()
    }

    val productJson: String
        get() = _productJson ?: "{}"

    suspend fun restoreTransactions(): Boolean {
        var result = false
        try {
            val inventory = _helper.queryInventory(true, ArrayList())
            _logger.debug("${this::restoreTransactions.name}: successful")
            _inventory = inventory
            result = true
        } catch (ex: IabException) {
            _logger.debug("${this::restoreTransactions.name}: failed: ${ex.localizedMessage}")
        }
        return result
    }

    private suspend fun reconcileFailedPurchaseWithInventory(suspectBadPurchase: PurchaseFailureDescription) {
        var notified = false
        try {
            val hadPurchase = _inventory.hasPurchase(suspectBadPurchase.productId)
            val response = _helper.queryPurchases(_inventory, SkuType.INAPP)
            if (response != BillingResponseCode.OK) {
                _logger.debug("Received bad response from queryPurchases")
            }
            val hasPurchase = _inventory.hasPurchase(suspectBadPurchase.productId)
            if (!hadPurchase && !hasPurchase || hadPurchase && hasPurchase) {
                _unityPurchasing.onPurchaseFailed(suspectBadPurchase)
                notified = true
            } else if (!hadPurchase && hasPurchase) {
                val purchase = _inventory.getPurchase(suspectBadPurchase.productId)
                    ?: throw IllegalStateException("Unexpected state")
                val skuDetails = _inventory.getSkuDetails(suspectBadPurchase.productId)
                    ?: throw IllegalStateException("Unexpected state")
                _unityPurchasing.onPurchaseSucceeded(purchase.sku, encodeReceipt(purchase, skuDetails),
                    purchase.orderId.ifEmpty {
                        purchase.purchaseToken
                    })
                notified = true
            }
            if (!notified) {
                notifyUnityOfProducts(_inventory)
            }
        } catch (ex: RemoteException) {
            ex.printStackTrace()
            if (!notified) {
                _unityPurchasing.onPurchaseFailed(suspectBadPurchase)
            }
        } catch (ex: JsonException) {
            ex.printStackTrace()
            if (!notified) {
                _unityPurchasing.onPurchaseFailed(suspectBadPurchase)
            }
        }
    }

    private suspend fun queryInventory(skus: List<String>, delayDuration: Long) {
        delay(delayDuration)
        try {
            val inventory = _helper.queryInventory(true, skus)
            _logger.debug("${this::queryInventory.name}: successful")
            _inventory = inventory
            skus.forEach { sku ->
                if (_inventory.hasPurchase(sku)) {
                    return@forEach
                }
                val record = _inventory.getHistoryPurchase(sku) ?: return@forEach
                _scope.launch {
                    try {
                        // Silently consume in background.
                        _helper.consume(SkuType.INAPP, record.purchaseToken, record.sku)
                    } catch (ex: Exception) {
                        ex.printStackTrace()
                    }
                }
            }
            notifyUnityOfProducts(_inventory)
        } catch (ex: IabException) {
            val delay = _offlineBackOffTime
            _offlineBackOffTime = min(300000, delay * 2)
            _logger.info("${this::queryInventory.name}: failed ${ex.localizedMessage}, retry in ${delay}ms")
            queryInventory(skus, delay)
        }
    }

    private fun findPurchaseByOrderId(orderId: String): Pair<String, Purchase>? {
        return _inventory.getAllPurchases().find { item ->
            orderId == item.second.orderId.ifEmpty {
                item.second.purchaseToken
            }
        }
    }

    private fun notifyUnityOfProducts(inventory: Inventory) {
        val descriptions: MutableList<ProductDescription> = ArrayList()
        val products: MutableMap<String, String> = HashMap()
        inventory.skuMap.forEach { entry ->
            val details = entry.value
            val sku = details.sku
            products[sku] = details.originalJson
            val metadata = ProductMetadata(
                details.price,
                details.title,
                details.description,
                details.priceCurrencyCode,
                details.priceAmountMicros / 1000000)
            val purchase = inventory.getPurchase(sku)
            val description = if (purchase != null) {
                val receipt = encodeReceipt(purchase, details)
                val transactionId = purchase.orderId.ifEmpty {
                    purchase.purchaseToken
                }
                ProductDescription(sku, metadata, receipt, transactionId)
            } else {
                ProductDescription(sku, metadata, "", "")
            }
            descriptions.add(description)
        }
        _productJson = products.serialize()
        _unityPurchasing.onProductsRetrieved(descriptions)
    }

    private fun encodeReceipt(purchase: Purchase, skuDetails: SkuDetails): String {
        @Serializable
        @Suppress("unused")
        class Data(
            val json: String,
            val signature: String,
            val skuDetails: String,
            val isPurchaseHistorySupported: Boolean
        )

        val data = Data(
            purchase.originalJson,
            purchase.signature,
            skuDetails.originalJson,
            _helper.subscriptionPurchaseHistorySupported)
        return data.serialize()
    }

    override fun retrieveProducts(products: List<ProductDefinition>) {
        val skus = products.map { it.storeSpecificId }
        _scope.launch {
            if (_helper.setupDone) {
                _logger.debug("retrieveProducts: request ${skus.size} products")
                queryInventory(skus, 0)
            } else {
                val result = _helper.startSetup()
                _logger.debug("retrieveProducts: setup ${result.response}")
                if (result.isFailure) {
                    _logger.debug("retrieveProducts: failed to setup")
                    _unityPurchasing.onSetupFailed(InitializationFailureReason.PurchasingUnavailable)
                } else {
                    _logger.debug("retrieveProducts: request ${skus.size} products")
                    queryInventory(skus, 0)
                }
            }
        }
    }

    private suspend fun consumeSuspectFailedPurchase(product: ProductDefinition) {
        val sku = product.storeSpecificId
        _suspectFailedConsumableSkus.remove(sku)
        try {
            _inventory = _helper.queryInventory(false, _inventory.getAllSkus(SkuType.INAPP))
            val record = _inventory.getHistoryPurchase(sku)
            if (record != null) {
                try {
                    // Silently consume.
                    _helper.consume(SkuType.INAPP, record.purchaseToken, record.sku)
                } catch (ex: Exception) {
                    ex.printStackTrace()
                }
                purchase(product)
            } else {
                purchase(product)
            }
        } catch (ex: IabException) {
            val delay = _offlineBackOffTime
            _offlineBackOffTime = min(300000, delay * 2)
            _logger.info("${this::consumeSuspectFailedPurchase}: failed ${ex.localizedMessage}, retry in ${delay}ms")
            queryInventory(_inventory.getAllSkus(SkuType.INAPP), delay)
        }
    }

    override fun purchase(product: ProductDefinition) {
        if (_purchaseInProgress) {
            val description = PurchaseFailureDescription(product.storeSpecificId,
                PurchaseFailureReason.ExistingPurchasePending)
            _unityPurchasing.onPurchaseFailed(description)
            return
        }
        if (product.type == ProductType.Consumable &&
            _suspectFailedConsumableSkus.contains(product.storeSpecificId) &&
            !_inventory.hasPurchase(product.storeSpecificId)) {
            _scope.launch {
                consumeSuspectFailedPurchase(product)
            }
            return
        }
        val productId = product.storeSpecificId
        val details = _inventory.getSkuDetails(productId)
            ?: throw IllegalStateException("Product not found ${product.storeSpecificId}")
        _logger.debug("purchase: sku = $productId type = ${details.type}")
        _purchaseInProgress = true
        _scope.launch {
            try {
                val purchase = _plugin.purchase(productId)
                _logger.debug("purchase: successful")
                _purchaseInProgress = false
                _inventory.addPurchase(details.type, purchase)
                if (details.type == SkuType.SUBS) {
                    _inventory.addPurchaseToSubscriptionPurchaseHistory(purchase.sku)
                }
                _unityPurchasing.onPurchaseSucceeded(purchase.sku, encodeReceipt(purchase, details),
                    purchase.orderId.ifEmpty {
                        purchase.purchaseToken
                    })
            } catch (ex: StoreException) {
                val responseCode = ex.responseCode
                _logger.debug("Purchase response code: $responseCode")
                _suspectFailedConsumableSkus.add(productId)
                val reason = when (responseCode) {
                    -1005, 1 -> PurchaseFailureReason.UserCancelled
                    2, 3 -> PurchaseFailureReason.BillingUnavailable
                    4 -> PurchaseFailureReason.ItemUnavailable
                    7 -> PurchaseFailureReason.DuplicateTransaction
                    else -> PurchaseFailureReason.Unknown
                }
                val description = PurchaseFailureDescription(
                    productId, reason, "GOOGLEPLAY_${ex.message}",
                    _billingResponseCodeNames[responseCode] ?: "")
                if (responseCode == -1002) {
                    _logger.debug("Received bad response, polling for successful purchases to investigate failure more deeply")
                    reconcileFailedPurchaseWithInventory(description)
                } else {
                    _unityPurchasing.onPurchaseFailed(description)
                }
            }
        }
    }

    override fun finishTransaction(product: ProductDefinition, transactionId: String) {
        _logger.debug("finishTransaction: $transactionId")
        val item = findPurchaseByOrderId(transactionId) ?: return
        val type = item.first
        val purchase = item.second
        if (product.type == ProductType.Consumable) {
            _logger.debug("finishTransaction: consuming ${purchase.sku}")
            _inventory.erasePurchase(purchase.sku)
            _scope.launch {
                _helper.consume(type, purchase.purchaseToken, purchase.sku)
            }
        } else {
            _logger.debug("finishTransaction: acknowledging ${purchase.sku}")
            _scope.launch {
                _helper.acknowledge(purchase.purchaseToken, purchase.sku)
            }
        }
    }

    suspend fun finishAdditionalTransaction(product: String, transactionId: String) {
        val item = findPurchaseByOrderId(transactionId) ?: return
        val type = item.first
        val purchase = item.second
        _logger.debug("finishTransaction: consuming ${purchase.sku}")
        _inventory.erasePurchase(purchase.sku)
        _helper.consume(type, purchase.purchaseToken, purchase.sku)
    }
}