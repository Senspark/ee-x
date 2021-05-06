package com.ee.internal

import kotlinx.serialization.builtins.ListSerializer
import kotlinx.serialization.json.Json

abstract class StoreDeserializer : INativeStore, IStore {
    override fun retrieveProducts(json: String) {
        val products = Json.decodeFromString(ListSerializer(ProductDefinition.serializer()), json)
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