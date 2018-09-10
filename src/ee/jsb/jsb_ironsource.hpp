//
// jsb_ironsource.cpp
// ee_x
//
//  Created by Nguyen Van Quynh on 8/24/18
//
//

#ifndef JSB_IRON_SOURCE_HPP__
#define JSB_IRON_SOURCE_HPP__

#ifdef __cplusplus
#include "ee/jsb/ironsource/jsb_iron_source_bridge.hpp"

namespace ee {
namespace ironsource {
bool register_iron_source_manual(se::Object* object) {
    register_iron_source_bridge_manual(object);
    return true;
} // namespace register_unity_ads_manual
} // namespace ironsource
} // namespace ee

#endif // __cplusplus
#endif // JSB_IRON_SOURCE_HPP__
