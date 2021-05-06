package com.ee.internal

import kotlinx.serialization.builtins.ListSerializer
import kotlinx.serialization.json.Json

class UnityPurchasing(private val _bridge: IUnityCallback) : IStoreCallback {
    override fun onSetupFailed(reason: InitializationFailureReason) {
        _bridge.onSetupFailed("$reason")
    }

    override fun onProductsRetrieved(products: List<ProductDescription>) {
        _bridge.onProductsRetrieved(Json.encodeToString(ListSerializer(ProductDescription.serializer()), products))
    }

    override fun onPurchaseSucceeded(storeSpecificId: String, receipt: String, transactionId: String) {
        _bridge.onPurchaseSucceeded(storeSpecificId, receipt, transactionId)
    }

    override fun onPurchaseFailed(description: PurchaseFailureDescription) {
        _bridge.onPurchaseFailed(description.serialize())
    }
}