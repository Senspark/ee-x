//
//  CCLifetimeVGBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "CCLifetimeVG.h"
#include "CCLifetimeVGBuilder.h"

namespace soomla {

using Self = CCLifetimeVGBuilder;

Self::CCLifetimeVGBuilder() {}
Self::~CCLifetimeVGBuilder() {}

Self& Self::setPurchaseType(CCPurchaseType* type) {
    type_ = cocos2d::RefPtr<CCPurchaseType>{type};
    return *this;
}

Self& Self::setName(const std::string& name) {
    name_ = std::make_unique<std::string>(name);
    return *this;
}

Self& Self::setDescription(const std::string& description) {
    description_ = std::make_unique<std::string>(description);
    return *this;
}

Self& Self::setItemId(const std::string& itemId) {
    itemId_ = std::make_unique<std::string>(itemId);
    return *this;
}

CCVirtualItem* Self::build() {
    CC_ASSERT(name_);
    CC_ASSERT(description_);
    CC_ASSERT(itemId_);
    CC_ASSERT(type_);
    return CCLifetimeVG::create(*name_, *description_, *itemId_, type_);
}
} // namespace soomla
