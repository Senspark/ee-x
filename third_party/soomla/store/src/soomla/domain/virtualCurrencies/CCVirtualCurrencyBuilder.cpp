//
//  CCVirtualCurrencyPackBuilder.cpp
//  ee-x
//
//  Created by Kiet Le on 9/19/18.
//

#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyBuilder.h"

namespace soomla {
using Self = VirtualCurrencyBuilder;

CCVirtualCurrency* Self::build() const {
    return CCVirtualCurrency::create(name_, description_, *itemId_);
}
} // namespace soomla
