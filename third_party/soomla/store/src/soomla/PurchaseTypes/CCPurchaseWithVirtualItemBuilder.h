#ifndef SOOMLA_STORE_PURCHASE_WITH_VIRTUAL_ITEM_BUILDER_HPP
#define SOOMLA_STORE_PURCHASE_WITH_VIRTUAL_ITEM_BUILDER_HPP

#ifdef __cplusplus

#include <optional>
#include <string>

#include <soomla/PurchaseTypes/CCPurchaseWithVirtualItem.h>

namespace soomla {
class PurchaseWithVirtualItemBuilder {
private:
    using Self = PurchaseWithVirtualItemBuilder;

public:
    Self& setItemId(const std::string& itemId);
    Self& setAmount(int amount);

    CCPurchaseWithVirtualItem* build() const;

private:
    std::optional<std::string> itemId_;
    std::optional<int> amount_;
};

// Backward compatibility.
using CCPurchaseWithVirtualItemBuilder = PurchaseWithVirtualItemBuilder;
} // namespace soomla

#endif // __cplusplus

#endif /* SOOMLA_STORE_PURCHASE_WITH_VIRTUAL_ITEM_BUILDER_HPP */
