//
//  CCPurchaseWithMarketBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "CCPurchaseWithMarket.h"
#include "CCPurchaseWithMarketBuilder.h"

namespace soomla {

using Self = CCPurchaseWithMarketBuilder;

Self::CCPurchaseWithMarketBuilder() {}
Self::~CCPurchaseWithMarketBuilder() {}

Self& Self::setProductId(const std::string& productId) {
    productId_ = std::make_unique<std::string>(productId);
    return *this;
}

Self& Self::setPrice(float price) {
    price_ = std::make_unique<float>(price);
    return *this;
}

CCPurchaseType* Self::build() {
    CC_ASSERT(productId_);
    CC_ASSERT(price_);
    return CCPurchaseWithMarket::create(*productId_, *price_);
}
} // namespace soomla
