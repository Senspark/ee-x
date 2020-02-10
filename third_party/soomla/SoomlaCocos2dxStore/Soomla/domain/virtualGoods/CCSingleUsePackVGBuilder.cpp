#include "Soomla/domain/virtualGoods/CCSingleUsePackVGBuilder.h"

namespace soomla {
using Self = CCSingleUsePackVGBuilder;

Self& Self::setGoodAmount(int goodAmount) {
    goodAmount_ = goodAmount;
    return *this;
}

Self& Self::setGoodId(const std::string& goodId) {
    goodId_ = goodId;
    return *this;
}

CCSingleUsePackVG* Self::build() const {
    return CCSingleUsePackVG::create(*goodId_, *goodAmount_, name_,
                                     description_, *itemId_, type_);
}
} // namespace soomla