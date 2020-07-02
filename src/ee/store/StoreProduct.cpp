//
//  StoreProduct.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/StoreProduct.hpp"

namespace ee {
namespace store {
using Self = Product;

Self::Product(const std::shared_ptr<ProductDefinition>& definition,
              const std::shared_ptr<ProductMetadata>& metadata,
              const std::string& receipt)
    : definition_(definition)
    , metadata_(metadata)
    , receipt_(receipt) {}

const std::shared_ptr<ProductDefinition>& Self::definition() const {
    return definition_;
}

const std::shared_ptr<ProductMetadata>& Self::metadata() const {
    return metadata_;
}

bool Self::availableToPurchase() const {
    return availableToPurchase_;
}

const std::string& Self::transactionId() const {
    return transactionId_;
}

const std::string& Self::receipt() const {
    return receipt_;
}
} // namespace store
} // namespace ee
