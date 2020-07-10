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
#include <vector>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class ConfigurationBuilder {
private:
    using Self = ConfigurationBuilder;

public:
    const std::set<std::shared_ptr<ProductDefinition>>& products() const;
    
    static Self standardInstance();

    Self& addProduct(const std::string& id, ProductType type,
                     const std::string& storeIds);

private:
    friend Bridge;
    friend UnityPurchasing;

    static Self
    instance(const std::shared_ptr<IPurchasingModule>& first,
             const std::vector<std::shared_ptr<IPurchasingModule>>& rest = {});

    explicit ConfigurationBuilder(
        const std::shared_ptr<PurchasingFactory>& factory);

    std::shared_ptr<PurchasingFactory> factory_;
    std::set<std::shared_ptr<ProductDefinition>> products_;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_CONFIGURATION_BUILDER_HPP */
