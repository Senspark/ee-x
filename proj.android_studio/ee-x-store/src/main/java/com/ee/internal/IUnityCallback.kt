package com.ee.internal

interface IUnityCallback {
    fun onSetupFailed(json: String)
    fun onProductsRetrieved(json: String)
    fun onPurchaseSucceeded(id: String, receipt: String, transactionId: String)
    fun onPurchaseFailed(json: String)
}