#include "soomla/domain/CCPurchasableVirtualItemBuilder.h"

#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPackBuilder.h"
#include "soomla/domain/virtualGoods/CCLifetimeVGBuilder.h"
#include "soomla/domain/virtualGoods/CCSingleUseVGBuilder.h"

namespace soomla {
template <class T>
using Self = PurchasableVirtualItemBuilder<T>;

template <class T>
T& Self<T>::setPurchaseType(CCPurchaseType* type) {
    type_ = type;
    return static_cast<T&>(*this);
}

template class PurchasableVirtualItemBuilder<VirtualCurrencyPackBuilder>;
template class PurchasableVirtualItemBuilder<LifetimeVGBuilder>;
template class PurchasableVirtualItemBuilder<SingleUseVGBuilder>;
} // namespace soomla
