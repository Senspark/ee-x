//
//  CCVirtualCurrencyPackBuilder.hpp
//  ee-x
//
//  Created by Nguyen Van Quynh on 9/14/18.
//

#ifndef SOOMLA_VIRTUAL_CURRENCY_PACK_BUILDER_HPP
#define SOOMLA_VIRTUAL_CURRENCY_PACK_BUILDER_HPP

#include "soomla/domain/CCPurchasableVirtualItemBuilder.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPack.h"

namespace soomla {
class VirtualCurrencyPackBuilder
    : public PurchasableVirtualItemBuilder<VirtualCurrencyPackBuilder> {
private:
    using Self = VirtualCurrencyPackBuilder;

public:
    Self& setCurrencyAmount(int currencyAmount);
    Self& setCurrencyItemId(const std::string& currencyItemId);

    virtual CCVirtualCurrencyPack* build() const override;

protected:
    std::optional<int> currencyAmount_;
    std::optional<std::string> currencyItemId_;
};

/// Backward compatibility.
using CCVirtualCurrencyPackBuilder = VirtualCurrencyPackBuilder;
} // namespace soomla

#endif /* SOOMLA_VIRTUAL_CURRENCY_PACK_BUILDER_HPP */
