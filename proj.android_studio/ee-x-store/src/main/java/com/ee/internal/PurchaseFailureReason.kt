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
enum class PurchaseFailureReason {
    BillingUnavailable,
    ExistingPurchasePending,
    ItemUnavailable,
    SignatureInvalid,
    UserCancelled,
    PaymentDeclined,
    DuplicateTransaction,
    Unknown;

    @Serializer(forClass = PurchaseFailureReason::class)
    companion object : KSerializer<PurchaseFailureReason> {
        override val descriptor: SerialDescriptor
            get() = PrimitiveDescriptor("com.ee.internal.PurchaseFailureReason", PrimitiveKind.INT)

        override fun serialize(encoder: Encoder, value: PurchaseFailureReason) {
            encoder.encodeInt(value.ordinal)
        }

        override fun deserialize(decoder: Decoder): PurchaseFailureReason {
            return PurchaseFailureReason.values()[decoder.decodeInt()]
        }
    }
}