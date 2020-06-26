//
//  StoreSimpleCatalogProvider.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_SIMPLE_CATALOG_PROVIDER_HPP
#define EE_X_STORE_SIMPLE_CATALOG_PROVIDER_HPP

#ifdef __cplusplus

#include "ee/store/StoreICatalogProvider.hpp"

namespace ee {
namespace store {
class SimpleCatalogProvider : public ICatalogProvider {
public:
    virtual Task<std::set<ProductDefinition>> fetchProducts() override;

private:
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_SIMPLE_CATALOG_PROVIDER_HPP */
