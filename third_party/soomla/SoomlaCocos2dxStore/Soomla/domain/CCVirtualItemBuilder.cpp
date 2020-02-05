//
//  CCVirtualItemBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "Soomla/domain/CCVirtualItemBuilder.h"

namespace soomla {

using Self = CCVirtualItemBuilder;

Self::CCVirtualItemBuilder() {}

Self::~CCVirtualItemBuilder() {}

CCVirtualItem* Self::build() {
    return virtualItem_.get();
}

Self& Self::setName(const std::string& name) {
    virtualItem_->name_ = std::make_unique<std::string>(name);
    return *this;
}

Self& Self::setDescription(const std::string& description) {
    virtualItem_->description_ = std::make_unique<std::string>(description);
    return *this;
}

Self& Self::setItemId(const std::string& itemId) {
    virtualItem_->itemId_ = std::make_unique<std::string>(itemId);
    return *this;
}
} // namespace soomla
