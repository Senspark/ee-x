#include "soomla/domain/virtualGoods/CCSingleUseVGBuilder.h"

namespace soomla {
using Self = SingleUseVGBuilder;

CCSingleUseVG* Self::build() const {
    return CCSingleUseVG::create(name_, description_, *itemId_, type_);
}
} // namespace soomla
