//
//  CCVirtualItemBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "soomla/domain/CCVirtualItemBuilder.h"

#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyBuilder.h"
#include "soomla/domain/virtualCurrencies/CCVirtualCurrencyPackBuilder.h"
#include "soomla/domain/virtualGoods/CCLifetimeVGBuilder.h"
#include "soomla/domain/virtualGoods/CCSingleUseVGBuilder.h"

namespace soomla {
template <class T>
using Self = VirtualItemBuilder<T>;

template <class T>
T& Self<T>::setName(const std::string& name) {
    name_ = name;
    return static_cast<T&>(*this);
}

template <class T>
T& Self<T>::setDescription(const std::string& description) {
    description_ = description;
    return static_cast<T&>(*this);
}

template <class T>
T& Self<T>::setItemId(const std::string& itemId) {
    itemId_ = itemId;
    return static_cast<T&>(*this);
}

template class VirtualItemBuilder<VirtualCurrencyBuilder>;
template class VirtualItemBuilder<VirtualCurrencyPackBuilder>;
template class VirtualItemBuilder<LifetimeVGBuilder>;
template class VirtualItemBuilder<SingleUseVGBuilder>;
} // namespace soomla
