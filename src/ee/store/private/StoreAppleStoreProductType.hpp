//
//  StoreAppleStoreProductType.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_APPLE_STORE_PRODUCT_TYPE_HPP
#define EE_X_STORE_APPLE_STORE_PRODUCT_TYPE_HPP

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class AppleStoreProductType {
    NonConsumable,
    Consumable,
    NonRenewingSubscription,
    AutoRenewingSubscription,
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_APPLE_STORE_PRODUCT_TYPE_HPP */
