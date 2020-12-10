package com.ee.internal

import kotlinx.serialization.InternalSerializationApi

class UnityPurchasing(private val _bridge: IUnityCallback) : IStoreCallback {
    override fun onSetupFailed(reason: InitializationFailureReason) {
        _bridge.onSetupFailed("$reason")
    }

    @InternalSerializationApi
    override fun onProductsRetrieved(products: List<ProductDescription>) {
        _bridge.onProductsRetrieved(products.serialize())
    }

    override fun onPurchaseSucceeded(storeSpecificId: String, receipt: String, transactionId: String) {
        _bridge.onPurchaseSucceeded(storeSpecificId, receipt, transactionId)
    }

    @InternalSerializationApi
    override fun onPurchaseFailed(description: PurchaseFailureDescription) {
        _bridge.onPurchaseFailed(description.serialize())
    }
}