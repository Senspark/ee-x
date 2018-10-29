//
//  CCLifetimeVGBuilder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#ifndef CCLifetimeVGBuilder_hpp
#define CCLifetimeVGBuilder_hpp

#include "../../PurchaseTypes/CCPurchaseType.h"
#include "../CCVirtualItem.h"

namespace soomla {

class CCLifetimeVGBuilder {
private:
    using Self = CCLifetimeVGBuilder;

public:
    CCLifetimeVGBuilder();
    ~CCLifetimeVGBuilder();

    Self& setPurchaseType(CCPurchaseType* type);

    Self& setName(const std::string& name);

    Self& setDescription(const std::string& description);

    Self& setItemId(const std::string& itemId);

    CCVirtualItem* build();

private:
    cocos2d::RefPtr<CCPurchaseType> type_;
    std::unique_ptr<std::string> name_;
    std::unique_ptr<std::string> description_;
    std::unique_ptr<std::string> itemId_;
};

} // namespace soomla

#endif /* CCLifetimeVGBuilder_hpp */
