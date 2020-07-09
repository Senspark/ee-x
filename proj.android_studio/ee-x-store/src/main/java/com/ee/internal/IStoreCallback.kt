package com.ee.internal

interface IStoreCallback {
    fun onSetupFailed(reason: InitializationFailureReason)
    fun onProductsRetrieved(products: List<ProductDescription>)
    fun onPurchaseSucceeded(storeSpecificId: String, receipt: String, transactionId: String)
    fun onPurchaseFailed(description: PurchaseFailureDescription)
}
