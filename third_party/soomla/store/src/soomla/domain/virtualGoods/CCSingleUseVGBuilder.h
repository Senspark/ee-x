#ifndef SOOMLA_SINGLE_USE_VG_BUILDER_HPP
#define SOOMLA_SINGLE_USE_VG_BUILDER_HPP

#ifdef __cplusplus

#include "soomla/domain/CCPurchasableVirtualItemBuilder.h"
#include "soomla/domain/virtualGoods/CCSingleUseVG.h"

namespace soomla {
class SingleUseVGBuilder
    : public PurchasableVirtualItemBuilder<SingleUseVGBuilder> {
public:
    virtual CCSingleUseVG* build() const override;
};

// Backward compatibility.
using CCSingleUseVGBuilder = SingleUseVGBuilder;
} // namespace soomla

#endif // __cplusplus

#endif // SOOMLA_SINGLE_USE_VG_BUILDER_HPP
