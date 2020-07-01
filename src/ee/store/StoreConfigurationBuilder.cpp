//
//  StoreConfigurationBuilder.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/StoreConfigurationBuilder.hpp"

#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/private/StorePurchasingFactory.hpp"

namespace ee {
namespace store {
using Self = ConfigurationBuilder;

std::shared_ptr<Self>
Self::instance(const std::shared_ptr<IPurchasingModule>& first,
               const std::vector<std::shared_ptr<IPurchasingModule>>& rest) {
    return std::shared_ptr<Self>(
        new Self(std::make_shared<PurchasingFactory>(first, rest)));
}

Self& Self::addProduct(const std::string& id, ProductType type,
                       const std::string& storeIds) {
    products_.insert(std::make_shared<ProductDefinition>(id, storeIds, type));
    return *this;
}

const std::set<std::shared_ptr<ProductDefinition>>& Self::products() const {
    return products_;
}
} // namespace store
} // namespace ee
