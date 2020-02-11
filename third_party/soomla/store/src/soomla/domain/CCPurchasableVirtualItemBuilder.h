#ifndef SOOMLA_PURCHASABLE_VIRTUAL_ITEM_BUILDER_HPP
#define SOOMLA_PURCHASABLE_VIRTUAL_ITEM_BUILDER_HPP

#include <base/CCRefPtr.h>

#include "soomla/domain/CCVirtualItemBuilder.h"
#include "soomla/PurchaseTypes/CCPurchaseType.h"

namespace soomla {
template <class T>
class PurchasableVirtualItemBuilder : public VirtualItemBuilder<T> {
public:
    T& setPurchaseType(CCPurchaseType* type);

protected:
    cocos2d::RefPtr<CCPurchaseType> type_;
};
} // namespace soomla

#endif // SOOMLA_PURCHASABLE_VIRTUAL_ITEM_BUILDER_HPP
