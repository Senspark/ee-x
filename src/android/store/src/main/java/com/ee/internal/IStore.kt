package com.ee.internal

interface IStore {
    fun retrieveProducts(products: List<ProductDefinition>)
    fun purchase(product: ProductDefinition)
    fun finishTransaction(product: ProductDefinition, transactionId: String)
}