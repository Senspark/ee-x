package com.ee.internal

import kotlinx.serialization.InternalSerializationApi

@InternalSerializationApi
abstract class StoreDeserializer : INativeStore, IStore {
    override fun retrieveProducts(json: String) {
        val products = deserializeList<ProductDefinition>(json)
        retrieveProducts(products)
    }

    override fun purchase(productJson: String) {
        val product = deserialize<ProductDefinition>(productJson)
        purchase(product)
    }

    override fun finishTransaction(productJson: String, transactionId: String) {
        val product = deserialize<ProductDefinition>(productJson)
        finishTransaction(product, transactionId)
    }
}