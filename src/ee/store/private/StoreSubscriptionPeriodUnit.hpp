//
//  StoreSubscriptionPeriodUnit.hpp
//  Pods
//
//  Created by eps on 7/3/20.
//

#ifndef EE_X_STORE_SUBSCRIPTION_PERIOD_UNIT_HPP
#define EE_X_STORE_SUBSCRIPTION_PERIOD_UNIT_HPP

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
enum class SubscriptionPeriodUnit {
    Day,
    Month,
    Week,
    Year,
    NotAvailable,
};
} // namespace store
} // namespace ee

#endif /* EE_X_STORE_SUBSCRIPTION_PERIOD_UNIT_HPP */
