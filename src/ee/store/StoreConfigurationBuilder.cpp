//
//  StoreConfigurationBuilder.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/StoreConfigurationBuilder.hpp"

#include "ee/store/StoreProductDefinition.hpp"

namespace ee {
namespace store {
using Self = ConfigurationBuilder;

Self& Self::addProduct(const std::string& id, ProductType type,
                       const std::string& storeIds) {
    products_.insert(std::make_shared<ProductDefinition>(id, storeIds, type));
    return *this;
}

const std::set<std::shared_ptr<ProductDefinition>>& Self::getProducts() const {
    return products_;
}
} // namespace store
} // namespace ee
