//
//  CCVirtualCurrencyPackBuilder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "CCVirtualCurrencyPackBuilder.h"

namespace soomla {

using Self = CCVirtualCurrencyPackBuilder;

Self::CCVirtualCurrencyPackBuilder() {}

Self::~CCVirtualCurrencyPackBuilder() {}

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

Self& Self::setCurrencyAmount(int currencyAmount) {
    currencyAmount_ = std::make_unique<int>(currencyAmount);
    return *this;
}

Self& Self::setCurrencyItemId(const std::string& currencyItemId) {
    currencyItemId_ = std::make_unique<std::string>(currencyItemId);
    return *this;
}

Self& Self::setPurchaseType(CCPurchaseType* type) {
    type_ = cocos2d::RefPtr<soomla::CCPurchaseType>{type};
    return *this;
}

CCVirtualCurrencyPack* Self::build() {
    CC_ASSERT(name_);
    CC_ASSERT(description_);
    CC_ASSERT(itemId_);
    CC_ASSERT(currencyAmount_);
    CC_ASSERT(currencyItemId_);
    CC_ASSERT(type_);
    return CCVirtualCurrencyPack::create(*name_, *description_, *itemId_,
                                         *currencyAmount_, *currencyItemId_,
                                         type_);
}
} // namespace soomla
