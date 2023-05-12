package com.ee.internal

import kotlinx.serialization.ExperimentalSerializationApi
import kotlinx.serialization.KSerializer
import kotlinx.serialization.Serializable
import kotlinx.serialization.Serializer
import kotlinx.serialization.descriptors.PrimitiveKind
import kotlinx.serialization.descriptors.PrimitiveSerialDescriptor
import kotlinx.serialization.descriptors.SerialDescriptor
import kotlinx.serialization.encoding.Decoder
import kotlinx.serialization.encoding.Encoder

@ExperimentalSerializationApi
@Serializer(forClass = ProductType::class)
object ProductTypeSerializer : KSerializer<ProductType> {
    override val descriptor: SerialDescriptor
        get() = PrimitiveSerialDescriptor("com.ee.internal.ProductType", PrimitiveKind.INT)

    override fun serialize(encoder: Encoder, value: ProductType) {
        encoder.encodeInt(value.ordinal)
    }

    override fun deserialize(decoder: Decoder): ProductType {
        return ProductType.values()[decoder.decodeInt()]
    }
}

@OptIn(ExperimentalSerializationApi::class)
@Serializable(with = ProductTypeSerializer::class)
enum class ProductType {
    Consumable,
    NonConsumable,
    Subscription;
}