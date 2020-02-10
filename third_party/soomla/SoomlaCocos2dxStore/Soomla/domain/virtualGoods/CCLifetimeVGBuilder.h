//
//  CCLifetimeVGBuilder.hpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#ifndef SOOMLA_LIFETIME_VG_BUILDER_HPP
#define SOOMLA_LIFETIME_VG_BUILDER_HPP

#include "Soomla/domain/CCPurchasableVirtualItemBuilder.h"
#include "Soomla/domain/virtualGoods/CCLifetimeVG.h"

namespace soomla {
class LifetimeVGBuilder
    : public PurchasableVirtualItemBuilder<LifetimeVGBuilder> {
private:
    using Self = LifetimeVGBuilder;

public:
    virtual CCLifetimeVG* build() const override;
};

// Backward compatibility.
using CCLifetimeVGBuilder = LifetimeVGBuilder;
} // namespace soomla

#endif /* SOOMLA_LIFETIME_VG_BUILDER_HPP */
