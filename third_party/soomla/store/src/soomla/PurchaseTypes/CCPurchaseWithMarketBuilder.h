//
//  CCPurchaseWithMarketBuilder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#ifndef SOOMLA_PURCHASE_WITH_MARKET_BUILDER_HPP
#define SOOMLA_PURCHASE_WITH_MARKET_BUILDER_HPP

#include <optional>
#include <string>

#include "soomla/PurchaseTypes/CCPurchaseWithMarket.h"

namespace soomla {
class PurchaseWithMarketBuilder {
private:
    using Self = PurchaseWithMarketBuilder;

public:
    Self& setProductId(const std::string& productId);
    Self& setPrice(float price);

    CCPurchaseWithMarket* build() const;
    
private:
    std::optional<std::string> productId_;
    float price_; // Optional.
};

// Backward compatibility.
using CCPurchaseWithMarketBuilder = PurchaseWithMarketBuilder;
} // namespace soomla

#endif /* CCPurchaseWithMarketBuilder_hpp */
