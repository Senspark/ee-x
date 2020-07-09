package com.ee.internal

interface INativeStore {
    fun retrieveProducts(json: String)
    fun purchase(productJson: String)
    fun finishTransaction(productJson: String, transactionId: String)
}