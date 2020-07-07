package com.ee.internal

import kotlinx.serialization.ImplicitReflectionSerializer
import kotlinx.serialization.UnstableDefault

abstract class StoreDeserializer : INativeStore, IStore {
    @ImplicitReflectionSerializer
    @UnstableDefault
    override fun retrieveProducts(json: String) {
        val products = deserializeList<ProductDefinition>(json)
        retrieveProducts(products)
    }

    @ImplicitReflectionSerializer
    @UnstableDefault
    override fun purchase(productJson: String) {
        val product = deserialize<ProductDefinition>(productJson)
        purchase(product)
    }

    @ImplicitReflectionSerializer
    @UnstableDefault
    override fun finishTransaction(productJson: String, transactionId: String) {
        val product = deserialize<ProductDefinition>(productJson)
        finishTransaction(product, transactionId)
    }
}