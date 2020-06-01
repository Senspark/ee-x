#ifndef EE_X_SKU_DETAILS_HPP
#define EE_X_SKU_DETAILS_HPP

#include <string>

#include "ee/StoreFwd.hpp"

namespace ee {
namespace store {
struct SkuDetails {
    std::string description;
    std::string freeTrialPeriod;
    std::string price;
    std::int64_t priceAmountMicros;
    std::string priceCurrencyCode;
    std::string sku;
    std::string subscriptionPeriod;
    std::string title;
};
} // namespace store
} // namespace ee

#endif // EE_X_SKU_DETAILS_HPP
