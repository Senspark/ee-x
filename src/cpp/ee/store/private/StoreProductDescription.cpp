//
//  StoreProductDescription.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/private/StoreProductDescription.hpp"

namespace ee {
namespace store {
using Self = ProductDescription;

Self::ProductDescription(const std::string& id,
                         const std::shared_ptr<ProductMetadata>& metadata,
                         const std::string& receipt,
                         const std::string& transactionId, ProductType type)
    : type_(type)
    , storeSpecificId_(id)
    , metadata_(metadata)
    , receipt_(receipt)
    , transactionId_(transactionId) {}

ProductType Self::type() const {
    return type_;
}

const std::string& Self::storeSpecificId() const {
    return storeSpecificId_;
}

const std::shared_ptr<ProductMetadata>& Self::metadata() const {
    return metadata_;
}

const std::string& Self::receipt() const {
    return receipt_;
}

const std::string& Self::transactionId() const {
    return transactionId_;
}
} // namespace store
} // namespace ee
