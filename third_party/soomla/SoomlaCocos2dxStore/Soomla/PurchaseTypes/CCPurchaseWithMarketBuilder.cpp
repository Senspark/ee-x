//
//  CCPurchaseWithMarketBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "Soomla/PurchaseTypes/CCPurchaseWithMarketBuilder.h"

namespace soomla {
using Self = PurchaseWithMarketBuilder;

Self& Self::setProductId(const std::string& productId) {
    productId_ = productId;
    return *this;
}

Self& Self::setPrice(float price) {
    price_ = price;
    return *this;
}

CCPurchaseWithMarket* Self::build() const {
    return CCPurchaseWithMarket::create(*productId_, price_);
}
} // namespace soomla
