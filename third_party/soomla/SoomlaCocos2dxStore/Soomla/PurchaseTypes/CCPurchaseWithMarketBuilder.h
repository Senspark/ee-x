//
//  CCPurchaseWithMarketBuilder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#ifndef CCPurchaseWithMarketBuilder_hpp
#define CCPurchaseWithMarketBuilder_hpp

#include "CCPurchaseType.h"

namespace soomla {

class CCPurchaseWithMarketBuilder {
private:
    using Self = CCPurchaseWithMarketBuilder;

public:
    CCPurchaseWithMarketBuilder();
    ~CCPurchaseWithMarketBuilder();

    Self& setProductId(const std::string& productId);

    Self& setPrice(float price);

    CCPurchaseType* build();

private:
    std::unique_ptr<std::string> productId_;
    std::unique_ptr<float> price_;
};

} // namespace soomla

#endif /* CCPurchaseWithMarketBuilder_hpp */
