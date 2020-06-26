//
//  StoreConfigurationBuilder.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_CONFIGURATION_BUILDER_HPP
#define EE_X_STORE_CONFIGURATION_BUILDER_HPP

#ifdef __cplusplus

#include <set>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class ConfigurationBuilder {
private:
    using Self = ConfigurationBuilder;

public:
    Self& addProduct(const std::string& id, ProductType type,
                     const std::string& storeIds);

    const std::set<std::shared_ptr<ProductDefinition>>& getProducts() const;

private:
    std::set<std::shared_ptr<ProductDefinition>> products_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_CONFIGURATION_BUILDER_HPP */
