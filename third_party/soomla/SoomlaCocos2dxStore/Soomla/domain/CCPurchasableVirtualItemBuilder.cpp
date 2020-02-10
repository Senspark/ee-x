#include "Soomla/domain/CCPurchasableVirtualItemBuilder.h"

#include "Soomla/domain/virtualCurrencies/CCVirtualCurrencyPackBuilder.h"
#include "Soomla/domain/virtualGoods/CCLifetimeVGBuilder.h"
#include "Soomla/domain/virtualGoods/CCSingleUseVGBuilder.h"

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
