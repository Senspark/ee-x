#ifndef SOOMLA_SINGLE_USE_PACK_VG_BUILDER_HPP
#define SOOMLA_SINGLE_USE_PACK_VG_BUILDER_HPP

#include "soomla/domain/CCPurchasableVirtualItemBuilder.h"
#include "soomla/domain/virtualGoods/CCSingleUsePackVG.h"

namespace soomla {
class SingleUsePackVGBuilder
    : public PurchasableVirtualItemBuilder<SingleUsePackVGBuilder> {
private:
    using Self = SingleUsePackVGBuilder;

public:
    Self& setGoodAmount(int goodAmount);
    Self& setGoodId(const std::string& goodId);

    virtual CCSingleUsePackVG* build() const override;

private:
    std::optional<int> goodAmount_;
    std::optional<std::string> goodId_;
};

// Backward compatibility.
using CCSingleUsePackVGBuilder = SingleUsePackVGBuilder;
} // namespace soomla

#endif // SOOMLA_SINGLE_USE_PACK_VG_BUILDER_HPP
