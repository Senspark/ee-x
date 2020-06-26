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
} // namespace store
} // namespace ee
