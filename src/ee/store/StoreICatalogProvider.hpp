//
//  StoreICatalogProvider.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_I_CATALOG_PROVIDER_HPP
#define EE_X_STORE_I_CATALOG_PROVIDER_HPP

#ifdef __cplusplus

#include <set>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
class ICatalogProvider {
public:
    virtual ~ICatalogProvider() = default;
    virtual Task<std::set<ProductDefinition>> fetchProducts() = 0;
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_I_CATALOG_PROVIDER_HPP */
