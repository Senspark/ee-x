//
//  StoreProductDefinition.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/StoreProductDefinition.hpp"

#include "ee/store/StoreProductType.hpp"

namespace ee {
namespace store {
using Self = ProductDefinition;

Self::ProductDefinition()
    : Self("", "", ProductType::Consumable) {}

Self::ProductDefinition(const std::string& id,
                        const std::string& storeSpecificId, ProductType type)
    : id_(id)
    , storeSpecificId_(storeSpecificId)
    , type_(type) {}

Self::ProductDefinition(const std::string& id, ProductType type)
    : Self(id, id, type) {}

bool operator<(const Self& lhs, const Self& rhs) {
    return lhs.id() < rhs.id();
}
} // namespace store
} // namespace ee
