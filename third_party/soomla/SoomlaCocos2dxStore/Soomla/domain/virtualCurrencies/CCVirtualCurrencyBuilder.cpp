//
//  CCVirtualCurrencyPackBuilder.cpp
//  ee-x
//
//  Created by Kiet Le on 9/19/18.
//

#include "CCVirtualCurrencyBuilder.h"

namespace soomla {

using Self = CCVirtualCurrencyBuilder;

Self::CCVirtualCurrencyBuilder() {}

Self::~CCVirtualCurrencyBuilder() {}

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
    return CCVirtualCurrency::create(*name_, *description_, *itemId_);
}
} // namespace soomla
