package com.ee.internal

import kotlinx.serialization.Decoder
import kotlinx.serialization.Encoder
import kotlinx.serialization.KSerializer
import kotlinx.serialization.PrimitiveDescriptor
import kotlinx.serialization.PrimitiveKind
import kotlinx.serialization.SerialDescriptor
import kotlinx.serialization.Serializable
import kotlinx.serialization.Serializer

@Serializable
enum class ProductType {
    Consumable,
    NonConsumable,
    Subscription;

    @Serializer(forClass = ProductType::class)
    companion object : KSerializer<ProductType> {
        override val descriptor: SerialDescriptor
            get() = PrimitiveDescriptor("com.ee.internal.ProductType", PrimitiveKind.INT)

        override fun serialize(encoder: Encoder, value: ProductType) {
            encoder.encodeInt(value.ordinal)
        }

        override fun deserialize(decoder: Decoder): ProductType {
            return ProductType.values()[decoder.decodeInt()]
        }
    }
}