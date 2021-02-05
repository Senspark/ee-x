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

@Serializable
enum class PurchaseFailureReason {
    BillingUnavailable,
    ExistingPurchasePending,
    ItemUnavailable,
    SignatureInvalid,
    UserCancelled,
    PaymentDeclined,
    DuplicateTransaction,
    Unknown;

    @ExperimentalSerializationApi
    @Serializer(forClass = PurchaseFailureReason::class)
    companion object : KSerializer<PurchaseFailureReason> {
        override val descriptor: SerialDescriptor
            get() = PrimitiveSerialDescriptor("com.ee.internal.PurchaseFailureReason", PrimitiveKind.INT)

        override fun serialize(encoder: Encoder, value: PurchaseFailureReason) {
            encoder.encodeInt(value.ordinal)
        }

        override fun deserialize(decoder: Decoder): PurchaseFailureReason {
            return values()[decoder.decodeInt()]
        }
    }
}