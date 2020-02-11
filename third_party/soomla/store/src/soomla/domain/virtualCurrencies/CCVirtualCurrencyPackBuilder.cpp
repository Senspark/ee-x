//
//  CCVirtualCurrencyPackBuilder.cpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPackBuilder.h"

namespace soomla {
using Self = CCVirtualCurrencyPackBuilder;

Self& Self::setCurrencyAmount(int currencyAmount) {
    currencyAmount_ = currencyAmount;
    return *this;
}

Self& Self::setCurrencyItemId(const std::string& currencyItemId) {
    currencyItemId_ = currencyItemId;
    return *this;
}

CCVirtualCurrencyPack* Self::build() const {
    return CCVirtualCurrencyPack::create(name_, description_, *itemId_,
                                         *currencyAmount_, *currencyItemId_,
                                         type_);
}
} // namespace soomla
