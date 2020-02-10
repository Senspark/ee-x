//
//  CCLifetimeVGBuilder.cpp
//  ee-x
//
//  Created by Le Van Kiet on 9/14/18.
//

#include "Soomla/domain/virtualGoods/CCLifetimeVGBuilder.h"

namespace soomla {
using Self = LifetimeVGBuilder;

CCLifetimeVG* Self::build() const {
    return CCLifetimeVG::create(name_, description_, *itemId_, type_);
}
} // namespace soomla
