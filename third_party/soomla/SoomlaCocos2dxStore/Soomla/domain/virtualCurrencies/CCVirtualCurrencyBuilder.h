//
//  CCVirtualCurrencyBuilder.hpp
//  ee-x
//
//  Created by Kiet Le on 9/19/18.
//

#ifndef SOOMLA_VIRTUAL_CURRENCY_BUILDER_HPP
#define SOOMLA_VIRTUAL_CURRENCY_BUILDER_HPP

#include "Soomla/domain/CCVirtualItemBuilder.h"
#include "Soomla/domain/virtualCurrencies/CCVirtualCurrency.h"

namespace soomla {
class VirtualCurrencyBuilder
    : public VirtualItemBuilder<VirtualCurrencyBuilder> {
private:
    using Self = VirtualCurrencyBuilder;

public:
    virtual CCVirtualCurrency* build() const override;
};

/// Backward compatibility.
using CCVirtualCurrencyBuilder = VirtualCurrencyBuilder;
} // namespace soomla

#endif /* SOOMLA_VIRTUAL_CURRENCY_BUILDER_HPP */
