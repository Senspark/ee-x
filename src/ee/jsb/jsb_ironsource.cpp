//
//  jsb_ironsource.cpp
//  ee-x
//
//  Created by Quynhnv on 10/24/18.
//

#include "ee/jsb/jsb_ironsource.hpp"

#include "ee/jsb/ironsource/jsb_iron_source_bridge.hpp"

namespace ee {
namespace ironsource {
bool register_iron_source_manual(se::Object* object) {
    register_iron_source_bridge_manual(object);
    return true;
}
} // namespace ironsource
} // namespace ee
