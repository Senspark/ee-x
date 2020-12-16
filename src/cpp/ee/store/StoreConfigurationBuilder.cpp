//
//  StoreConfigurationBuilder.cpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#include "ee/store/StoreConfigurationBuilder.hpp"

#include "ee/store/StoreProductDefinition.hpp"
#include "ee/store/private/StorePurchasingFactory.hpp"
#include "ee/store/private/StoreStandardPurchasingModule.hpp"

namespace ee {
namespace store {
using Self = ConfigurationBuilder;

Self::ConfigurationBuilder(const std::shared_ptr<PurchasingFactory>& factory)
    : factory_(factory) {}

const std::set<std::shared_ptr<ProductDefinition>>& Self::products() const {
    return products_;
}

Self Self::standardInstance() {
    return instance(StandardPurchasingModule::instance());
}

Self Self::instance(
    const std::shared_ptr<IPurchasingModule>& first,
    const std::vector<std::shared_ptr<IPurchasingModule>>& rest) {
    return Self(PurchasingFactory::create(first, rest));
}

Self& Self::addProduct(const std::string& id, ProductType type,
                       const std::string& storeIds) {
    products_.insert(std::make_shared<ProductDefinition>(id, storeIds, type));
    return *this;
}

} // namespace store
} // namespace ee
