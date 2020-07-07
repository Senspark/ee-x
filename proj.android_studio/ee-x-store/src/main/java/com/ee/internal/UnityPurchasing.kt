package com.ee.internal

import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault

class UnityPurchasing(private val _bridge: IUnityCallback) : IStoreCallback {
    override fun onSetupFailed(reason: InitializationFailureReason) {
        _bridge.onSetupFailed("$reason")
    }

    @ImplicitReflectionSerializer
    @UnstableDefault
    override fun onProductsRetrieved(products: List<ProductDescription>) {
        _bridge.onProductsRetrieved(products.serialize())
    }

    override fun onPurchaseSucceeded(storeSpecificId: String, receipt: String, transactionId: String) {
        _bridge.onPurchaseSucceeded(storeSpecificId, receipt, transactionId)
    }

    @ImplicitReflectionSerializer
    @UnstableDefault
    override fun onPurchaseFailed(description: PurchaseFailureDescription) {
        _bridge.onPurchaseFailed(description.serialize())
    }
}