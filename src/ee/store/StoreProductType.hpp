//
//  StoreProductType.hpp
//  Pods
//
//  Created by eps on 6/26/20.
//

#ifndef EE_X_STORE_PRODUCT_TYPE_HPP
#define EE_X_STORE_PRODUCT_TYPE_HPP

#ifdef __cplusplus

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class ProductType {
    Consumable,
    NonConsumable,
    Subsription,
};
} // namespace store
} // namespace ee

#endif // __cplusplus

#endif /* EE_X_STORE_PRODUCT_TYPE_HPP */
