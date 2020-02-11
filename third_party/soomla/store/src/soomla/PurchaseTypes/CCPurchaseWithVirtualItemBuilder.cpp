#include "soomla/PurchaseTypes/CCPurchaseWithVirtualItemBuilder.h"

namespace soomla {
using Self = PurchaseWithVirtualItemBuilder;

Self& Self::setItemId(const std::string& itemId) {
    itemId_ = itemId;
    return *this;
}

Self& Self::setAmount(int amount) {
    amount_ = amount;
    return *this;
}

CCPurchaseWithVirtualItem* Self::build() const {
    return CCPurchaseWithVirtualItem::create(*itemId_, *amount_);
}
} // namespace soomla
