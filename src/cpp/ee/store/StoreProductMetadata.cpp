//
//  StoreProductMetadata.cpp
//  Pods
//
//  Created by eps on 7/2/20.
//

#include "ee/store/StoreProductMetadata.hpp"

namespace ee {
namespace store {
using Self = ProductMetadata;

Self::ProductMetadata()
    : Self("", "", "", "", 0) {}

Self::ProductMetadata(const std::string& priceString, const std::string& title,
                      const std::string& description,
                      const std::string& currencyCode, double localizedPrice)
    : localizedPriceString_(priceString)
    , localizedTitle_(title)
    , localizedDescription_(description)
    , isoCurrencyCode_(currencyCode)
    , localizedPrice_(localizedPrice) {}

const std::string& Self::localizedPriceString() const {
    return localizedPriceString_;
}

const std::string& Self::localizedTitle() const {
    return localizedTitle_;
}

const std::string& Self::localizedDescription() const {
    return localizedDescription_;
}

const std::string& Self::isoCurrencyCode() const {
    return isoCurrencyCode_;
}

double Self::localizedPrice() const {
    return localizedPrice_;
}
} // namespace store
} // namespace ee
